#include "tiki/materialconverter/toolmaterial.hpp"

#include "tiki/base/reflection.hpp"
#include "tiki/base/sort.hpp"
#include "tiki/base/stringparse.hpp"
#include "tiki/converterbase/converterhelper.hpp"
#include "tiki/converterbase/resourcewriter.hpp"
#include "tiki/io/xmlreader.hpp"

namespace tiki
{
	struct MaterialFieldSortData
	{
		uint							dataOffset;
		const reflection::FieldMember*	pField;

		bool operator>( const MaterialFieldSortData& rhs )
		{
			return dataOffset > rhs.dataOffset;
		}

		bool operator<( const MaterialFieldSortData& rhs )
		{
			return dataOffset < rhs.dataOffset;
		}

		bool operator>=( const MaterialFieldSortData& rhs )
		{
			return dataOffset >= rhs.dataOffset;
		}

		bool operator<=( const MaterialFieldSortData& rhs )
		{
			return dataOffset <= rhs.dataOffset;
		}
	};

	ToolMaterial::ToolMaterial()
	{
		m_pDataType		= nullptr;
	}

	ToolMaterial::~ToolMaterial()
	{
		TIKI_ASSERT( m_pDataType == nullptr );
	}

	bool ToolMaterial::create( const string& fileName )
	{
		XmlReader xml;
		xml.create( fileName.cStr() );

		const XmlElement* pRootNode = xml.findNodeByName( "tikimaterial" );
		if ( pRootNode == nullptr )
		{
			xml.dispose();
			return false;
		}
		const XmlElement* pTypeNode = xml.findFirstChild( "type", pRootNode );
		if ( pTypeNode == nullptr )
		{
			xml.dispose();
			return false;
		}

		const reflection::TypeBase* pTypeBase = reflection::getTypeOf( pTypeNode->content );
		if ( pTypeBase == nullptr || pTypeBase->getLeaf() != reflection::TypeBaseLeaf_StructType )
		{
			xml.dispose();
			return false;
		}
		m_pDataType = static_cast< const reflection::StructType* >( pTypeBase );
		m_effectData.create( m_pDataType->getFieldCount() );

		const XmlElement* pValueNode = xml.findFirstChild( "field", pRootNode );
		while ( pValueNode )
		{
			const XmlAttribute* pNameAtt = xml.findAttributeByName( "name", pValueNode );
			if ( pNameAtt != nullptr )
			{
				const XmlAttribute* pTypeAtt = xml.findAttributeByName( "type", pValueNode );
				if ( pTypeAtt != nullptr )
				{
					const string typeString = pTypeAtt->content;
					MaterialFieldType type = MaterialFieldType_Invalid;
					if ( typeString == "resource" )
					{
						type = MaterialFieldType_Reference;
					}
					else if ( typeString == "integer" )
					{
						type = MaterialFieldType_Integer;
					}
					else if ( typeString == "float" )
					{
						type = MaterialFieldType_Float;
					}
					else
					{
						TIKI_TRACE_ERROR( "[toolmaterial] Field with name '%s' has a invalid type '%s'.\n", pNameAtt->content, typeString.cStr() );
					}

					const reflection::FieldMember* pField = m_pDataType->getFieldByName( pNameAtt->content );
					if ( pField != nullptr && type != MaterialFieldType_Invalid )
					{
						MaterialField fieldInfo;
						fieldInfo.name	= pNameAtt->content;
						fieldInfo.type	= type;
						fieldInfo.value	= pValueNode->content;

						m_effectData.set( pField, fieldInfo );
					}
					else if ( type != MaterialFieldType_Invalid )
					{
						TIKI_TRACE_ERROR( "[toolmaterial] Field with name '%s' is not a member of Type: '%s'.\n", pNameAtt->content, m_pDataType->getName().cStr() );
					}
				}
				else
				{
					TIKI_TRACE_ERROR( "[toolmaterial] Field with name '%s' has no 'type' attribute.\n", pNameAtt->content );
				}
			}
			else
			{
				TIKI_TRACE_ERROR( "[toolmaterial] Could not find 'name' attribute of field.\n" );
			}

			pValueNode = xml.findNext( "field", pValueNode );
		}

		xml.dispose();
		return true;
	}

	void ToolMaterial::dispose()
	{
		m_pDataType = nullptr;
		m_effectData.dispose();
	}

	ReferenceKey ToolMaterial::writeResource( ResourceWriter& writer )
	{
		void* pData = m_pDataType->createInstance();

		List< MaterialFieldSortData > sortDataList;
		for (uint i = 0u; i < m_pDataType->getFieldCount(); ++i)
		{
			MaterialFieldSortData& data = sortDataList.add();
			data.dataOffset	= m_pDataType->getFieldByIndex( i )->getOffset();
			data.pField		= m_pDataType->getFieldByIndex( i );
		}
		quickSort( sortDataList.getBegin(), sortDataList.getCount() );
		
		for (uint i = 0u; i < m_effectData.getCount(); ++i)
		{
			MaterialFieldMap::Pair& kvp = m_effectData.getPairAt( i );
			
			switch ( kvp.value.type )
			{
			case MaterialFieldType_Reference:
				if ( !readResourceReference( writer, kvp.value.value, kvp.value.data.referenceKey ) )
				{
					TIKI_TRACE_ERROR( "[toolmaterial] Invalid reference value '%s'.\n", kvp.value.value.cStr() );
				}
				break;

			case MaterialFieldType_Integer:
				kvp.value.data.integerValue = ParseString::parseInt64( kvp.value.value );
				break;

			case MaterialFieldType_Float:
				kvp.value.data.floatValue = ParseString::parseDouble( kvp.value.value );
				break;
			}
		} 

		writer.openDataSection( 0u, AllocatorType_MainMemory );
		const ReferenceKey key = writer.addDataPoint();

		uint currentSectionSize = 0u;
		for (uint i = 0u; i < sortDataList.getCount(); ++i)
		{
			const MaterialFieldSortData& sortData = sortDataList[ i ];

			MaterialField field;
			if ( m_effectData.findValue( &field, sortData.pField ) )
			{
				const uint offsetDifference = sortData.dataOffset - currentSectionSize;
				if ( offsetDifference > 0u )
				{
					writer.writeData( addPtr( pData, currentSectionSize ), offsetDifference );
				}
				currentSectionSize = sortData.dataOffset;

				switch ( field.type )
				{
				case MaterialFieldType_Reference:
					writer.writeReference( &field.data.referenceKey );
					currentSectionSize += 8u;
					break;

				case MaterialFieldType_Integer:
					if ( sortData.pField->getTypeInfo().getType() == reflection::getTypeOf( "uint8" ) )
					{
						writer.writeUInt8( (uint8)field.data.integerValue );
						currentSectionSize += 1u;
					}
					else if ( sortData.pField->getTypeInfo().getType() == reflection::getTypeOf( "uint16" ) )
					{
						writer.writeUInt16( (uint16)field.data.integerValue );
						currentSectionSize += 2u;
					}
					else if ( sortData.pField->getTypeInfo().getType() == reflection::getTypeOf( "uint32" ) )
					{
						writer.writeUInt32( (uint32)field.data.integerValue );
						currentSectionSize += 4u;
					}
					else if ( sortData.pField->getTypeInfo().getType() == reflection::getTypeOf( "uint64" ) )
					{
						writer.writeUInt64( (uint64)field.data.integerValue );
						currentSectionSize += 8u;
					}
					else if ( sortData.pField->getTypeInfo().getType() == reflection::getTypeOf( "sint8" ) )
					{
						writer.writeSInt8( (sint8)field.data.integerValue );
						currentSectionSize += 1u;
					}
					else if ( sortData.pField->getTypeInfo().getType() == reflection::getTypeOf( "sint16" ) )
					{
						writer.writeSInt16( (sint16)field.data.integerValue );
						currentSectionSize += 2u;
					}
					else if ( sortData.pField->getTypeInfo().getType() == reflection::getTypeOf( "sint32" ) )
					{
						writer.writeSInt32( (sint32)field.data.integerValue );
						currentSectionSize += 4u;
					}
					else if ( sortData.pField->getTypeInfo().getType() == reflection::getTypeOf( "sint64" ) )
					{
						writer.writeSInt64( (sint64)field.data.integerValue );
						currentSectionSize += 8u;
					}
					break;

				case MaterialFieldType_Float:
					if ( sortData.pField->getTypeInfo().getType() == reflection::getTypeOf( "float" ) )
					{
						writer.writeFloat( (float)field.data.floatValue );
						currentSectionSize += 4u;
					}
					else if ( sortData.pField->getTypeInfo().getType() == reflection::getTypeOf( "double" ) )
					{
						writer.writeDouble( (float)field.data.floatValue );
						currentSectionSize += 8u;
					}
					break;
				}
			}		
		}
		writer.writeData( addPtr( pData, currentSectionSize ), m_pDataType->getSize() - currentSectionSize );
		
		writer.closeDataSection();

		m_pDataType->disposeInstance( pData );

		return key;
	}

}