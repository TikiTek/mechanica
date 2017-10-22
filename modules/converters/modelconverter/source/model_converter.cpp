#include "tiki/modelconverter/model_converter.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/base/float16.hpp"
#include "tiki/base/fourcc.hpp"
#include "tiki/base/string_tools.hpp"
#include "tiki/converterbase/conversion_asset.hpp"
#include "tiki/converterbase/converterhelper.hpp"
#include "tiki/converterbase/resource_writer.hpp"
#include "tiki/modelexport/toolmodel.hpp"

namespace tiki
{
	static const crc32 s_typeCrc = crcString( "model" );

	ModelConverter::ModelConverter()
	{
	}

	ModelConverter::~ModelConverter()
	{
	}

	uint32 ModelConverter::getConverterRevision( crc32 typeCrc ) const
	{
		return 6u;
	}

	bool ModelConverter::canConvertType( crc32 typeCrc ) const
	{
		return typeCrc == s_typeCrc;
	}

	crc32 ModelConverter::getOutputType() const
	{
		return s_typeCrc;
	}

	void ModelConverter::getDependingType( List< crc32 >& types ) const
	{
		types.add( crcString( "material" ) );
	}

	bool ModelConverter::initializeConverter()
	{
		return true;
	}

	void ModelConverter::disposeConverter()
	{
	}

	bool ModelConverter::startConversionJob( ConversionResult& result, const ConversionAsset& asset ) const
	{
		const string material	= asset.parameters.getOptionalString( "material", "" );
		const float scale		= asset.parameters.getOptionalFloat( "scale", 1.0f );
		const bool calcTangents	= asset.parameters.getOptionalBool( "calculate_tangents", true );

		ToolModel model;
		if( !model.create( asset.inputFilePath.getCompletePath(), scale ) ||
			!model.parseGeometies( calcTangents ) )
		{
			return false;
		}

		ResourceWriter writer;
		openResourceWriter( writer, result, asset.assetName, "model" );

		for( const ResourceDefinition& definition : getResourceDefinitions( FlagMask8< ResourceDefinitionFeature >() ) )
		{
			writer.openResource( asset.assetName + ".model", TIKI_FOURCC( 'M', 'O', 'D', 'L' ), definition, getConverterRevision( s_typeCrc ) );

			// write hierarchy
			const ReferenceKey* pHierarchyKey = nullptr;
			ReferenceKey hierarchyKey;
			if ( model.getHierarchy().isCreated() )
			{
				hierarchyKey = writeHierarchy( writer, model.getHierarchy() );
				pHierarchyKey = &hierarchyKey;
			}

			// write vertex data
			List< ReferenceKey > geometryKeys;

			bool wrongSkinned = false;
			for (uint geometryIndex = 0u; geometryIndex < model.getGeometyCount(); ++geometryIndex )
			{
				const ReferenceKey key = writeGeometry( writer, model.getGeometryByIndex( geometryIndex ) );
				geometryKeys.add( key );

				for (uint k = 0u; k < model.getGeometyCount(); ++k)
				{
					if ( model.getGeometryByIndex( geometryIndex ).getDesc().isSkinned != model.getGeometryByIndex( k ).getDesc().isSkinned )
					{
						wrongSkinned = true;
					}
				}
			}

			if ( wrongSkinned )
			{
				TIKI_TRACE_ERROR( "[modelconverter] Not every Mesh is skinned.\n" );
			}

			ResourceSectionWriter sectionWriter;
			writer.openDataSection( sectionWriter, SectionType_Initializaion );
			writeResourceReference( sectionWriter, material );
			sectionWriter.writeReference( pHierarchyKey );
			sectionWriter.writeUInt32( uint32( model.getGeometyCount() ) );
			for( uint geometryIndex = 0u; geometryIndex < geometryKeys.getCount(); ++geometryIndex )
			{
				sectionWriter.writeReference( &geometryKeys[ geometryIndex ] );
			}
			writer.closeDataSection( sectionWriter );
			writer.closeResource();
		}

		closeResourceWriter( writer );

		model.dispose();
		return true;
	}

	ReferenceKey ModelConverter::writeHierarchy( ResourceWriter& resourceWriter, const ToolModelHierarchy& hierarchy ) const
	{
		ResourceSectionWriter sectionWriter;
		resourceWriter.openDataSection( sectionWriter, SectionType_Main );

		const uint16 alignedJointCount = alignValue( (uint16)hierarchy.getJointCount(), (uint16)4u );

		const ReferenceKey jointNamesKey = sectionWriter.addDataPoint();
		for (uint j = 0u; j < hierarchy.getJointCount(); ++j)
		{
			sectionWriter.writeUInt32( hierarchy.getJointByIndex( j ).crc );
		}

		const ReferenceKey parentIndicesKey = sectionWriter.addDataPoint();
		for (uint j = 0u; j < hierarchy.getJointCount(); ++j)
		{
			sectionWriter.writeUInt16( uint16( hierarchy.getJointByIndex( j ).parentIndex ) );
		}

		Array< Quaternion > dpRotation;
		Array< Vector3 > dpPosition;
		Array< Vector3 > dpScale;
		dpRotation.create( alignedJointCount );
		dpPosition.create( alignedJointCount );
		dpScale.create( alignedJointCount );

		for (uint j = 0u; j < hierarchy.getJointCount(); ++j)
		{
			const ToolModelJoint& joint = hierarchy.getJointByIndex( j );
			matrix::decompose( dpRotation[ j ], dpPosition[ j ], dpScale[ j ], joint.defaultPose );
		}

		sectionWriter.writeAlignment( 16u );
		const ReferenceKey defaultPoseKey = sectionWriter.addDataPoint();
		for (uint j = 0u; j < alignedJointCount; ++j)
		{
			sectionWriter.writeFloat( dpRotation[ j ].x );
			sectionWriter.writeFloat( dpRotation[ j ].y );
			sectionWriter.writeFloat( dpRotation[ j ].z );
			sectionWriter.writeFloat( dpRotation[ j ].w );
		}

		for (uint j = 0u; j < alignedJointCount; ++j)
		{
			sectionWriter.writeFloat( dpPosition[ j ].x );
			sectionWriter.writeFloat( dpPosition[ j ].y );
			sectionWriter.writeFloat( dpPosition[ j ].z );
			sectionWriter.writeFloat( 0.0f );
		}

		for (uint j = 0u; j < alignedJointCount; ++j)
		{
			sectionWriter.writeFloat( dpScale[ j ].x );
			sectionWriter.writeFloat( dpScale[ j ].y );
			sectionWriter.writeFloat( dpScale[ j ].z );
			sectionWriter.writeFloat( 0.0f );
		}

		dpRotation.dispose();
		dpPosition.dispose();
		dpScale.dispose();

		sectionWriter.writeAlignment( 16u );
		const ReferenceKey skinToBoneKey = sectionWriter.addDataPoint();
		for (uint i = 0u; i < hierarchy.getJointCount(); ++i)
		{
			const ToolModelJoint& joint = hierarchy.getJointByIndex( i );
			sectionWriter.writeData( &joint.skinToBone.x.x, sizeof( Matrix44 ) );
		}

		const ReferenceKey refKey = sectionWriter.addDataPoint();
		sectionWriter.writeUInt16( uint16( hierarchy.getJointCount() ) );
		sectionWriter.writeUInt16( alignedJointCount );

		sectionWriter.writeReference( &jointNamesKey );
		sectionWriter.writeReference( &parentIndicesKey );
		sectionWriter.writeReference( &defaultPoseKey );
		sectionWriter.writeReference( &skinToBoneKey );

		resourceWriter.closeDataSection( sectionWriter );

		return refKey;
	}

	static void writeVertexAttribute( ResourceSectionWriter& sectionWriter, const uint8* pSource, VertexAttributeFormat targetFormat, bool isFloatFormat )
	{
		const uint elementCount = getVertexAttributeFormatElementCount( targetFormat );

		if ( isFloatFormat )
		{
			const float* pSourceData = (const float*)pSource;

			switch ( targetFormat )
			{
			case VertexAttributeFormat_x32y32z32w32_float:
			case VertexAttributeFormat_x32y32z32_float:
			case VertexAttributeFormat_x32y32_float:
			case VertexAttributeFormat_x32_float:
				{
					for (uint i = 0u; i < elementCount; ++i)
					{
						sectionWriter.writeFloat( pSourceData[ i ] );
					}
				}
				break;
			case VertexAttributeFormat_x16y16z16w16_float:
			case VertexAttributeFormat_x16y16_float:
			case VertexAttributeFormat_x16_float:
				{
					for (uint i = 0u; i < elementCount; ++i)
					{
						sectionWriter.writeUInt16( f16::convertFloat32to16( pSourceData[ i ] ) );
					}
				}
				break;
			case VertexAttributeFormat_x16y16z16w16_snorm:
			case VertexAttributeFormat_x16y16_snorm:
			case VertexAttributeFormat_x16_snorm:
				{
					for (uint i = 0u; i < elementCount; ++i)
					{
						TIKI_ASSERT( pSourceData[ i ] >= -1.0f && pSourceData[ i ] <= 1.0f );
						sectionWriter.writeSInt16( (sint16)( pSourceData[ i ] * 32667.0f ) );
					}
				}
				break;
			case VertexAttributeFormat_x16y16z16w16_unorm:
			case VertexAttributeFormat_x16y16_unorm:
			case VertexAttributeFormat_x16_unorm:
				{
					for (uint i = 0u; i < elementCount; ++i)
					{
						TIKI_ASSERT( pSourceData[ i ] >= 0.0f && pSourceData[ i ] <= 1.0f );
						sectionWriter.writeUInt16( (uint16)( pSourceData[ i ] * 65535.0f ) );
					}
				}
				break;
			case VertexAttributeFormat_x8y8z8w8_snorm:
				{
					for (uint i = 0u; i < elementCount; ++i)
					{
						TIKI_ASSERT( pSourceData[ i ] >= -1.0f && pSourceData[ i ] <= 1.0f );
						sectionWriter.writeUInt8( (sint8)( pSourceData[ i ] * 127.0f ) );
					}
				}
				break;
			case VertexAttributeFormat_x8y8z8w8_unorm:
				{
					for (uint i = 0u; i < elementCount; ++i)
					{
						TIKI_ASSERT( pSourceData[ i ] >= 0.0f && pSourceData[ i ] <= 1.0f );
						sectionWriter.writeUInt8( (uint8)( pSourceData[ i ] * 255.0f ) );
					}
				}
				break;
			}
		}
		else if ( targetFormat == VertexAttributeFormat_x8y8z8w8 )
		{
			const uint32* pSourceData = (const uint32*)pSource;

			for (uint i = 0u; i < elementCount; ++i)
			{
				TIKI_ASSERT( pSourceData[ i ] < 256u );
				sectionWriter.writeUInt8( (uint8)pSourceData[ i ] );
			}
		}
	}

	ReferenceKey ModelConverter::writeGeometry( ResourceWriter& resourceWriter, const ToolModelGeometrie& geometry ) const
	{
		const ToolModelVertexFormat& vertexFormat	= geometry.getVertexFormat();

		ResourceSectionWriter sectionWriter;
		resourceWriter.openDataSection( sectionWriter, SectionType_Main );

		sectionWriter.writeAlignment( 4u );
		const ReferenceKey vertexAttributesKey = sectionWriter.addDataPoint();
		sectionWriter.writeData(
			geometry.getVertexFormat().getAttributes(),
			geometry.getVertexFormat().getAttributeCount() * sizeof( VertexAttribute )
		);

		sectionWriter.writeAlignment( 16u );
		const ReferenceKey vertexDataKey = sectionWriter.addDataPoint();
		const VertexAttribute* pAttributes = vertexFormat.getAttributes();
		for (uint k = 0u; k < geometry.getVertexCount(); ++k)
		{
			const ToolModelVertex& vertex = geometry.getVertexByIndex( k );

			for (uint j = 0u; j < vertexFormat.getAttributeCount(); ++j)
			{
				const VertexAttribute& att = pAttributes[ j ];

				bool isFloat			= true;
				const uint8* pSource	= nullptr;

				switch ( att.semantic )
				{
				case VertexSementic_Position:
					pSource = (uint8*)&vertex.position.x;
					break;
				case VertexSementic_Normal:
					pSource = (uint8*)&vertex.normal.x;
					break;
				case VertexSementic_TangentFlip:
					pSource = (uint8*)&vertex.tangent.x;
					break;
				case VertexSementic_Binormal:
					pSource = (uint8*)&vertex.binormal.x;
					break;
				case VertexSementic_TexCoord:
					pSource = (uint8*)&vertex.texcoord.x;
					break;
				case VertexSementic_Color:
					pSource = (uint8*)&vertex.color.x;
					break;
				case VertexSementic_JointIndex:
					isFloat = false;
					pSource = (uint8*)&vertex.jointIndices.x;
					break;
				case VertexSementic_JointWeight:
					pSource = (uint8*)&vertex.jointWeights.x;
					break;
				}

				if ( pSource != nullptr )
				{
					writeVertexAttribute( sectionWriter, pSource, att.format, isFloat );
				}
			}
		}

		sectionWriter.writeAlignment( 4u );
		const ReferenceKey indexDataKey = sectionWriter.addDataPoint();
		for (uint k = 0u; k < geometry.getIndexCount(); ++k)
		{
			sectionWriter.writeUInt32( uint32( geometry.getIndexByIndex( k ) ) );
		}

		const ReferenceKey geometryKey = sectionWriter.addDataPoint();
		sectionWriter.writeUInt16( uint16( geometry.getVertexCount() ) );
		sectionWriter.writeUInt16( uint16( geometry.getIndexCount() ) );

		sectionWriter.writeUInt8( geometry.getDesc().isSkinned );
		sectionWriter.writeUInt8( uint8( vertexFormat.getVertexStride( 0u ) ) );
		sectionWriter.writeUInt8( 4u ); // index size
		sectionWriter.writeUInt8( uint8( vertexFormat.getAttributeCount() ) );

		sectionWriter.writeReference( &vertexAttributesKey );

		sectionWriter.writeReference( &vertexDataKey );
		sectionWriter.writeReference( &indexDataKey );

		resourceWriter.closeDataSection( sectionWriter );

		return geometryKey;
	}
}