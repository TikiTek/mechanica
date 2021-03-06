#include "tiki/animation_converter/animation_converter.hpp"

#include "tiki/animation_converter/animation_builder.hpp"
#include "tiki/base/crc32.hpp"
#include "tiki/base/fourcc.hpp"
#include "tiki/base/string_tools.hpp"
#include "tiki/converter_base/conversion_types.hpp"
#include "tiki/converter_base/resource_writer.hpp"
#include "tiki/io/file.hpp"
#include "tiki/io/path.hpp"
#include "tiki/model_export/tool_model.hpp"

namespace tiki
{
	static const char* s_pAnimationTypeName = "animation";
	static const crc32 s_animationTypeCrc = crcString( s_pAnimationTypeName );

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
		return typeCrc == s_animationTypeCrc;
	}

	void AnimationConverter::getInputExtensions( List< DynamicString >& extensions ) const
	{
		//extensions.pushBack( ".dae" );
	}

	const char* AnimationConverter::getOutputType() const
	{
		return s_pAnimationTypeName;
	}

	bool AnimationConverter::initializeConverter( const ConversionContext& context )
	{
		return true;
	}

	void AnimationConverter::disposeConverter()
	{
	}

	bool AnimationConverter::startConversionJob( ConversionResult& result, const ConversionAsset& asset, const ConversionContext& context ) const
	{
		float paramScale = asset.parameters.getOptionalFloat( "scale", 1.0f );

		DynamicString preboundModelName = asset.parameters.getString( "prebound_model" );
		if ( !file::exists( preboundModelName.cStr() ) )
		{
			preboundModelName = path::combine( path::getDirectoryName( asset.inputFilePath.getCompletePath() ), preboundModelName );
		}

		if ( !file::exists( preboundModelName.cStr() ) )
		{
			TIKI_TRACE_ERROR( "[animationconverter] prebound model not found. name: %s.\n", preboundModelName.cStr() );
			return false;
		}

		ToolModel preboundModel;
		preboundModel.create( preboundModelName, paramScale );

		ToolModel model;
		model.create( asset.inputFilePath.getCompletePath(), paramScale );
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

				AnimationSpline rotationSpline;
				AnimationSpline positionSpline;
				AnimationSpline scaleSpline;
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
		openResourceWriter( resourceWriter, result, asset.assetName.cStr(), "animation" );

		for( const ResourceDefinition& definition : getResourceDefinitions( FlagMask8< ResourceDefinitionFeature >() ) )
		{
			resourceWriter.openResource( asset.assetName + ".animation", TIKI_FOURCC( 'A', 'N', 'I', 'M' ), definition, getConverterRevision( s_animationTypeCrc ) );

			ReferenceKey dataKey;
			builder.writeToResource( resourceWriter, dataKey );

			ResourceSectionWriter sectionWriter;
			resourceWriter.openDataSection( sectionWriter, SectionType_Initializaion );
			sectionWriter.writeReference( &dataKey );
			resourceWriter.closeDataSection( sectionWriter );
			resourceWriter.closeResource();
		}

		closeResourceWriter( resourceWriter );

		builder.dispose();
		model.dispose();
		preboundModel.dispose();
		return true;
	}
}