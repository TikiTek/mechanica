
#include "tiki/modelconverter/modelconverter.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/base/float16.hpp"
#include "tiki/base/fourcc.hpp"
#include "tiki/base/stringparse.hpp"
#include "tiki/converterbase/conversionparameters.hpp"
#include "tiki/converterbase/converterhelper.hpp"
#include "tiki/converterbase/resourcewriter.hpp"
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
		return 5u;
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

	bool ModelConverter::startConversionJob( ConversionResult& result, const ConversionParameters& parameters ) const
	{
		for( uint fileIndex = 0u; fileIndex < parameters.inputFiles.getCount(); ++fileIndex )
		{
			const ConversionParameters::InputFile& file = parameters.inputFiles[ fileIndex ];

			const string material	= parameters.arguments.getOptionalString( "material", "" );
			const float scale		= parameters.arguments.getOptionalFloat( "scale", 1.0f );
			const bool calcTangents	= parameters.arguments.getOptionalBool( "calculate_tangents", true );

			ToolModel model;
			model.create( file.fileName, scale );
			model.parseGeometies( calcTangents );

			ResourceWriter writer;
			openResourceWriter( writer, result, parameters.outputName, "model", parameters.targetPlatform );
			
			writer.openResource( parameters.outputName + ".model", TIKI_FOURCC( 'M', 'O', 'D', 'L' ), getConverterRevision( s_typeCrc ) );

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

			writer.openDataSection( 0u, AllocatorType_InitializaionMemory );
			writeResourceReference( writer, material );
			writer.writeReference( pHierarchyKey );
			writer.writeUInt32( uint32( model.getGeometyCount() ) );
			for( uint geometryIndex = 0u; geometryIndex < geometryKeys.getCount(); ++geometryIndex )
			{
				writer.writeReference( &geometryKeys[ geometryIndex ] );
			}			
			writer.closeDataSection();

			writer.closeResource();
			closeResourceWriter( writer );

			model.dispose();
		}

		return true;
	}

	ReferenceKey ModelConverter::writeHierarchy( ResourceWriter& writer, const ToolModelHierarchy& hierarchy ) const
	{
		writer.openDataSection( 0u, AllocatorType_MainMemory );

		const uint16 alignedJointCount = alignValue( (uint16)hierarchy.getJointCount(), (uint16)4u );

		const ReferenceKey jointNamesKey = writer.addDataPoint();
		for (uint j = 0u; j < hierarchy.getJointCount(); ++j)
		{
			writer.writeUInt32( hierarchy.getJointByIndex( j ).crc );
		}

		const ReferenceKey parentIndicesKey = writer.addDataPoint();
		for (uint j = 0u; j < hierarchy.getJointCount(); ++j)
		{
			writer.writeUInt16( uint16( hierarchy.getJointByIndex( j ).parentIndex ) );
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

		writer.writeAlignment( 16u );
		const ReferenceKey defaultPoseKey = writer.addDataPoint();
		for (uint j = 0u; j < alignedJointCount; ++j)
		{
			writer.writeFloat( dpRotation[ j ].x );
			writer.writeFloat( dpRotation[ j ].y );
			writer.writeFloat( dpRotation[ j ].z );
			writer.writeFloat( dpRotation[ j ].w );
		}

		for (uint j = 0u; j < alignedJointCount; ++j)
		{
			writer.writeFloat( dpPosition[ j ].x );
			writer.writeFloat( dpPosition[ j ].y );
			writer.writeFloat( dpPosition[ j ].z );
			writer.writeFloat( 0.0f );
		}

		for (uint j = 0u; j < alignedJointCount; ++j)
		{
			writer.writeFloat( dpScale[ j ].x );
			writer.writeFloat( dpScale[ j ].y );
			writer.writeFloat( dpScale[ j ].z );
			writer.writeFloat( 0.0f );
		}

		dpRotation.dispose();
		dpPosition.dispose();
		dpScale.dispose();

		writer.writeAlignment( 16u );
		const ReferenceKey skinToBoneKey = writer.addDataPoint();
		for (uint i = 0u; i < hierarchy.getJointCount(); ++i)
		{
			const ToolModelJoint& joint = hierarchy.getJointByIndex( i );
			writer.writeData( &joint.skinToBone.x.x, sizeof( Matrix44 ) );
		}

		const ReferenceKey refKey = writer.addDataPoint();
		writer.writeUInt16( uint16( hierarchy.getJointCount() ) );
		writer.writeUInt16( alignedJointCount );

		writer.writeReference( &jointNamesKey );
		writer.writeReference( &parentIndicesKey );
		writer.writeReference( &defaultPoseKey );
		writer.writeReference( &skinToBoneKey );

		writer.closeDataSection();

		return refKey;
	}

	static void writeVertexAttribute( ResourceWriter& fileWriter, const uint8* pSource, VertexAttributeFormat targetFormat, bool isFloatFormat )
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
						fileWriter.writeFloat( pSourceData[ i ] );
					}
				}
				break;
			case VertexAttributeFormat_x16y16z16w16_float:
			case VertexAttributeFormat_x16y16_float:
			case VertexAttributeFormat_x16_float:
				{
					for (uint i = 0u; i < elementCount; ++i)
					{
						fileWriter.writeUInt16( f16::convertFloat32to16( pSourceData[ i ] ) );
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
						fileWriter.writeSInt16( (sint16)( pSourceData[ i ] * 32667.0f ) );
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
						fileWriter.writeUInt16( (uint16)( pSourceData[ i ] * 65535.0f ) );
					}
				}
				break;
			case VertexAttributeFormat_x8y8z8w8_snorm:
				{
					for (uint i = 0u; i < elementCount; ++i)
					{
						TIKI_ASSERT( pSourceData[ i ] >= -1.0f && pSourceData[ i ] <= 1.0f );
						fileWriter.writeUInt8( (sint8)( pSourceData[ i ] * 127.0f ) );
					}
				}
				break;
			case VertexAttributeFormat_x8y8z8w8_unorm:
				{
					for (uint i = 0u; i < elementCount; ++i)
					{
						TIKI_ASSERT( pSourceData[ i ] >= 0.0f && pSourceData[ i ] <= 1.0f );
						fileWriter.writeUInt8( (uint8)( pSourceData[ i ] * 255.0f ) );
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
				fileWriter.writeUInt8( (uint8)pSourceData[ i ] );
			}
		}
	}

	ReferenceKey ModelConverter::writeGeometry( ResourceWriter& writer, const ToolModelGeometrie& geometry ) const
	{
		const ToolModelVertexFormat& vertexFormat	= geometry.getVertexFormat();

		writer.openDataSection( 0u, AllocatorType_MainMemory );

		writer.writeAlignment( 4u );
		const ReferenceKey vertexAttributesKey = writer.addDataPoint();
		writer.writeData(
			geometry.getVertexFormat().getAttributes(),
			geometry.getVertexFormat().getAttributeCount() * sizeof( VertexAttribute )
		);

		writer.writeAlignment( 16u );
		const ReferenceKey vertexDataKey = writer.addDataPoint();
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
					writeVertexAttribute( writer, pSource, att.format, isFloat );
				}
			}
		}

		writer.writeAlignment( 4u );
		const ReferenceKey indexDataKey = writer.addDataPoint();
		for (uint k = 0u; k < geometry.getIndexCount(); ++k)
		{
			writer.writeUInt32( uint32( geometry.getIndexByIndex( k ) ) );
		}

		const ReferenceKey geometryKey = writer.addDataPoint();
		writer.writeUInt16( uint16( geometry.getVertexCount() ) );
		writer.writeUInt16( uint16( geometry.getIndexCount() ) );

		writer.writeUInt8( geometry.getDesc().isSkinned );
		writer.writeUInt8( uint8( vertexFormat.getVertexStride( 0u ) ) );
		writer.writeUInt8( 4u ); // index size
		writer.writeUInt8( uint8( vertexFormat.getAttributeCount() ) );

		writer.writeReference( &vertexAttributesKey );		

		writer.writeReference( &vertexDataKey );			
		writer.writeReference( &indexDataKey );

		writer.closeDataSection();

		return geometryKey;
	}
}