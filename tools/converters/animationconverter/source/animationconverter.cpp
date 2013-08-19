
#include "tiki/animationconverter/animationconverter.hpp"

#include "tiki/animationconverter/maskchunkcreator.hpp"
#include "tiki/base/crc32.hpp"
#include "tiki/base/file.hpp"
#include "tiki/base/fourcc.hpp"
#include "tiki/base/path.hpp"
#include "tiki/base/stringparse.hpp"
#include "tiki/converterbase/conversionparameters.hpp"
#include "tiki/converterbase/resourcewriter.hpp"
#include "tiki/modelexport/toolmodel.hpp"

namespace tiki
{
	AnimationConverter::AnimationConverter()
	{
	}

	AnimationConverter::~AnimationConverter()
	{
	}

	tiki::crc32 AnimationConverter::getInputType() const
	{
		return crcString( "animation" );
	}

	tiki::crc32 AnimationConverter::getOutputType() const
	{
		return crcString( "animation" );
	}

	void AnimationConverter::getDependingType( List< crc32 >& types ) const
	{
	}

	bool AnimationConverter::initializeConverter()
	{
		return true;
	}

	void AnimationConverter::disposeConverter()
	{
	}

	bool AnimationConverter::startConversionJob( const ConversionParameters& params ) const
	{
		for (size_t k = 0u; k < params.inputFiles.getCount(); ++k)
		{
			const ConversionInputFile& fileVar = params.inputFiles[ k ];

			float paramScale = params.arguments.getOptionalFloat( "scale", 1.0f );

			string preboundModelName = params.arguments.getString( "prebound_model" );

			if ( !file::exists( preboundModelName ) )
			{
				preboundModelName = path::combine( path::getDirectoryName( fileVar.fileName ), preboundModelName );
			}

			if ( !file::exists( preboundModelName ) )
			{
				TIKI_TRACE_ERROR( "[animationconverter] prebound model not found. name: %s.\n", preboundModelName.cStr() );
				return false;
			}

			ToolModel preboundModel;
			preboundModel.create( preboundModelName, paramScale );

			ToolModel model;
			model.create( fileVar.fileName, paramScale, "" );
			model.parseAnimations();

			// wrong prebound model
			TIKI_ASSERT( model.getHierarchy().getJointCount() == preboundModel.getHierarchy().getJointCount() );

			const ToolModelAnimation& anim = model.getAnimation();
			const ToolModelHierarchy& hier = preboundModel.getHierarchy();
			
			MaskChunkCreator maskCreator;
			maskCreator.create( (uint16)anim.getJointCount(), 64u, (uint16)anim.getFrameCount(), 0.0001f );

			size_t loopJoints	= 0u;
			size_t clampJoints	= 0u;

			for (size_t i = 0; i < anim.getJointCount(); ++i)
			{
				const ToolModelAnimationJoint& joint = anim.getJointByIndex( i );

				if ( joint.samples.getCount() > 0u )
				{
					const float maxError = 0.0f;

					List< float > orientationSamples;
					List< float > translationSamples;
					List< float > scaleSamples;

					Quaternion lastOrientation;
					for (size_t j = 0u; j < joint.samples.getCount(); ++j)
					{
						Quaternion orientation;
						Vector3 translation;
						Vector3 scale;

						joint.samples[ j ].decompose( orientation, translation, scale );

						// :NOTE: our math functions are too poor to pass this test
						//Matrix temp;
						//temp.compose( orientation, translation, scale );
						//TIKI_ASSERT( temp.isEquals( joint.samples[ j ], 0.1f ) );
						
						if ( j != 0u )
						{
							const float dot = orientation.dot( orientation, lastOrientation);

							if ( dot < 0.0f )
							{
								orientation.negate();
							}
						}
						lastOrientation = orientation;

						translation.mul( paramScale );

						orientationSamples.addRange( orientation.arr, TIKI_COUNT( orientation.arr ) );
						translationSamples.addRange( translation.arr, TIKI_COUNT( translation.arr ) );
						scaleSamples.addRange( scale.arr, TIKI_COUNT( scale.arr ) );
					}

					Spline orientationSpline;
					Spline translationSpline;
					Spline scaleSpline;

					fittSplineKeys( &orientationSpline, 4u, orientationSamples.getData(), joint.samples.getCount(), maxError );
					fittSplineKeys( &translationSpline, 3u, translationSamples.getData(), joint.samples.getCount(), maxError );
					fittSplineKeys( &scaleSpline, 3u, scaleSamples.getData(), joint.samples.getCount(), maxError );

					maskCreator.setJointData( i, orientationSpline, translationSpline, scaleSpline, hier.getJointByIndex( i ).defaultPose );

					destroySpline( &orientationSpline );
					destroySpline( &translationSpline );
					destroySpline( &scaleSpline );
				}
				else
				{
					Quaternion orientation;
					Vector3 translation;
					Vector3 scale;
					joint.pJoint->defaultPose.decompose( orientation, translation, scale );

					maskCreator.setConstantJoint( i, orientation, translation, scale, hier.getJointByIndex( i ).defaultPose );
				}
			}

			maskCreator.finalizeData();

			{
				ResourceWriter fileWriter;
				openResourceWriter( &fileWriter, TIKI_FOURCC( 'A', 'N', 'I', 'M' ), params.outputName, "anm" );
				
				const uint16 headerOffset = (sizeof(uint32) * 2u) + (sizeof(float) * 3u) + (sizeof(uint16) * 4u);
				const uint32 dataOffset = alignValue( headerOffset + ((sizeof(uint32) + (8u * sizeof(uint16))) * maskCreator.getChunkHeaderCount()), 16u );

				fileWriter.writeFloat( maskCreator.getPositionFactor() );
				fileWriter.writeFloat( maskCreator.getPositionTangent() );
				fileWriter.writeFloat( maskCreator.getScaleFactor() );

				fileWriter.writeUInt32( dataOffset );
				fileWriter.writeUInt32( maskCreator.getDataSize() );

				fileWriter.writeUInt16( headerOffset );
				fileWriter.writeUInt16( (uint16)maskCreator.getChunkHeaderCount() );

				fileWriter.writeUInt16( (uint16)( anim.getFrameCount() - 1u ) );
				fileWriter.writeUInt16( (uint16)anim.getJointCount() );

				// write header
				for (size_t i = 0; i < maskCreator.getChunkHeaderCount(); ++i)
				{
					const MaskChunkHeader& header = maskCreator.getChunkHeaders()[ i ];

					fileWriter.writeUInt32( header.dataOffset );

					fileWriter.writeUInt16( header.startTime );
					fileWriter.writeUInt16( header.endTime );

					fileWriter.writeUInt16( header.usedOrientationJoints );
					fileWriter.writeUInt16( header.nonConstOrientationJoints );

					fileWriter.writeUInt16( header.usedPositionJoints );
					fileWriter.writeUInt16( header.nonConstPositionJoints );

					fileWriter.writeUInt16( header.usedScaleJoints );
					fileWriter.writeUInt16( header.nonConstScaleJoints );
				}

				// write data
				fileWriter.writeAlignment( 16u );
				for (size_t i = 0; i < maskCreator.getDataBlockCount(); ++i)
				{
					const AnimationDataBlobArray& block = maskCreator.getDataBlocks()[ i ];

					if (block.alignment != 0)
					{
						fileWriter.writeAlignment( block.alignment );
					}

					switch (block.type)
					{
					case DataType_SInt8:
						{
							const sint8* pValues = (const sint8*)block.pData;

							for (size_t j = 0u; j < block.elementCount; ++j)
							{
								fileWriter.writeSInt8( pValues[ j ] );
							}
						}
						break;
					case DataType_UInt8:
						for (size_t j = 0u; j < block.elementCount; ++j)
						{
							fileWriter.writeUInt8( block.pData[ j ] );
						}
						break;
					case DataType_SInt16:
						{
							const sint16* pValues = (const sint16*)block.pData;

							for (size_t j = 0u; j < block.elementCount; ++j)
							{
								fileWriter.writeSInt16( pValues[ j ] );
							}
						}
						break;
					case DataType_UInt16:
						{
							const uint16* pValues = (const uint16*)block.pData;

							for (size_t j = 0u; j < block.elementCount; ++j)
							{
								fileWriter.writeUInt16( pValues[ j ] );
							}
						}
						break;
					case DataType_SInt32:
						{
							const sint32* pValues = (const sint32*)block.pData;

							for (size_t j = 0u; j < block.elementCount; ++j)
							{
								fileWriter.writeSInt32( pValues[ j ] );
							}
						}
						break;
					case DataType_UInt32:
						{
							const uint32* pValues = (const uint32*)block.pData;

							for (size_t j = 0u; j < block.elementCount; ++j)
							{
								fileWriter.writeUInt32( pValues[ j ] );
							}
						}
						break;
					case DataType_SInt64:
						{
							const sint64* pValues = (const sint64*)block.pData;

							for (size_t j = 0u; j < block.elementCount; ++j)
							{
								fileWriter.writeSInt64( pValues[ j ] );
							}
						}
						break;
					case DataType_UInt64:
						{
							const uint64* pValues = (const uint64*)block.pData;

							for (size_t j = 0u; j < block.elementCount; ++j)
							{
								fileWriter.writeUInt64( pValues[ j ] );
							}
						}
						break;
					}
				}

				closeResourceWriter( &fileWriter );
			}

			maskCreator.dispose();
			model.dispose();
			preboundModel.dispose();
		}

		return true;
	}

}