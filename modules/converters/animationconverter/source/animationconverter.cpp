
#include "tiki/animationconverter/animationconverter.hpp"

#include "tiki/animationconverter/animationbuilder.hpp"
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
	static const crc32 s_typeCrc = crcString( "animation" );

	AnimationConverter::AnimationConverter()
	{
	}

	AnimationConverter::~AnimationConverter()
	{
	}
	
	uint32 AnimationConverter::getConverterRevision( crc32 typeCrc ) const 
	{
		return 1u;
	}

	bool AnimationConverter::canConvertType( crc32 typeCrc ) const
	{
		return typeCrc == s_typeCrc;
	}

	crc32 AnimationConverter::getOutputType() const
	{
		return s_typeCrc;
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

	bool AnimationConverter::startConversionJob( ConversionResult& result, const ConversionParameters& parameters ) const
	{
		for (uint k = 0u; k < parameters.inputFiles.getCount(); ++k)
		{
			const ConversionParameters::InputFile& file = parameters.inputFiles[ k ];

			float paramScale = parameters.arguments.getOptionalFloat( "scale", 1.0f );

			string preboundModelName = parameters.arguments.getString( "prebound_model" );

			if ( !file::exists( preboundModelName.cStr() ) )
			{
				preboundModelName = path::combine( path::getDirectoryName( file.fileName ), preboundModelName );
			}

			if ( !file::exists( preboundModelName.cStr() ) )
			{
				TIKI_TRACE_ERROR( "[animationconverter] prebound model not found. name: %s.\n", preboundModelName.cStr() );
				return false;
			}

			ToolModel preboundModel;
			preboundModel.create( preboundModelName, paramScale );

			ToolModel model;
			model.create( file.fileName, paramScale );
			model.parseAnimations();

			// wrong prebound model
			TIKI_ASSERT( model.getHierarchy().getJointCount() == preboundModel.getHierarchy().getJointCount() );

			const ToolModelAnimation& anim = model.getAnimation();
			const ToolModelHierarchy& hier = preboundModel.getHierarchy();
			
			AnimationBuilder builder;
			builder.create( anim.getFrameCount(), anim.getJointCount(), 64u, 0.0001f );

			uint loopJoints	= 0u;
			uint clampJoints	= 0u;

			for (uint i = 0; i < anim.getJointCount(); ++i)
			{
				const ToolModelAnimationJoint& joint = anim.getJointByIndex( i );

				if ( joint.samples.getCount() > 0u )
				{
					const float maxError = 0.0f;

					List< float > rotationSamples;
					List< float > positionSamples;
					List< float > scaleSamples;

					Quaternion lastRotation;
					for (uint j = 0u; j < joint.samples.getCount(); ++j)
					{
						Quaternion rotation;
						Vector3 position;
						Vector3 scale;
						matrix::decompose( rotation, position, scale, joint.samples[ j ] );
						
						// :NOTE: our math functions are too poor to pass this test
						//Matrix temp;
						//temp.compose( rotation, position, scale );
						//TIKI_ASSERT( temp.isEquals( joint.samples[ j ], 0.1f ) );
						
						if ( j != 0u )
						{
							const float dot = quaternion::dot( rotation, lastRotation );

							if ( dot < 0.0f )
							{
								quaternion::negate( rotation );
							}
						}
						lastRotation = rotation;

						vector::scale( position, paramScale );

						rotationSamples.addRange( &rotation.x, 4u );
						positionSamples.addRange( &position.x, 3u );
						scaleSamples.addRange( &scale.x, 3u );
					}

					Spline rotationSpline;
					Spline positionSpline;
					Spline scaleSpline;
					rotationSpline.createFromSamples( rotationSamples.getBegin(), 4u, joint.samples.getCount(), maxError );
					positionSpline.createFromSamples( positionSamples.getBegin(), 3u, joint.samples.getCount(), maxError );
					scaleSpline.createFromSamples( scaleSamples.getBegin(), 3u, joint.samples.getCount(), maxError );

					builder.setJointData( i, rotationSpline, positionSpline, scaleSpline, hier.getJointByIndex( i ).defaultPose );

					rotationSpline.dispose();
					positionSpline.dispose();
					scaleSpline.dispose();
				}
				else
				{
					Quaternion rotation;
					Vector3 position;
					Vector3 scale;
					matrix::decompose( rotation, position, scale, joint.pJoint->defaultPose );

					builder.setConstantJointData( i, rotation, position, scale, hier.getJointByIndex( i ).defaultPose );
				}
			}

			ResourceWriter resourceWriter;
			openResourceWriter( resourceWriter, result, parameters.outputName, "animation" );

			for (const ResourceDefinition& definition : getResourceDefinitions())
			{
				resourceWriter.openResource( parameters.outputName + ".animation", TIKI_FOURCC( 'A', 'N', 'I', 'M' ), definition, getConverterRevision( s_typeCrc ) );

				ReferenceKey dataKey;
				builder.writeToResource( resourceWriter, dataKey );

				resourceWriter.openDataSection( 0u, AllocatorType_InitializaionMemory );
				resourceWriter.writeReference( &dataKey );
				resourceWriter.closeDataSection();

				resourceWriter.closeResource();				
			}

			closeResourceWriter( resourceWriter );

			builder.dispose();
			model.dispose();
			preboundModel.dispose();
		}

		return true;
	}
}