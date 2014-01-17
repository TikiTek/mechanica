#include "tiki/materialconverter/toolmaterial.hpp"

#include "tiki/base/reflection.hpp"
#include "tiki/converterbase/converterhelper.hpp"
#include "tiki/converterbase/resourcewriter.hpp"
#include "tiki/toolbase/tikixml.hpp"

namespace tiki
{
	struct MaterialDataReference
	{
		uint			dataOffset;
		ReferenceKey	referenceKey;
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
		TikiXml xml;
		xml.create( fileName );

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
					const reflection::FieldMember* pField = m_pDataType->getFieldByName( pNameAtt->content );
					if ( pField != nullptr )
					{
						MaterialField fieldInfo;
						fieldInfo.name	= pNameAtt->content;
						fieldInfo.type	= pTypeAtt->content;
						fieldInfo.value	= pValueNode->content;

						m_effectData.set( pField, fieldInfo );
					}
					else
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

	ReferenceKey ToolMaterial::writeResource( ResourceWriter& writer ) const
	{
		void* pData = m_pDataType->createInstance();

		uint8 workingMemory[ 16u ];
		List< MaterialDataReference > references;
		for (uint i = 0u; i < m_effectData.getCount(); ++i)
		{
			const MaterialFieldMap::Pair& kvp = m_effectData.getPairAt( i );
			
			TIKI_ASSERT( sizeof( workingMemory ) >= kvp.key->getTypeInfo().getType()->getSize() );
			memory::zero( workingMemory, sizeof( workingMemory ) );

			if ( kvp.value.type == "resource" )
			{
				MaterialDataReference reference;
				reference.dataOffset = kvp.key->getOffset();
				if ( readResourceReference( writer, kvp.value.value, reference.referenceKey ) )
				{
					references.add( reference );
				}
			}
			else if ( kvp.value.type == "integer" )
			{
				//static_cast< uint64* >
			}
			else if ( kvp.value.type == "float" )
			{

			}
			else
			{
				TIKI_TRACE_ERROR( "[toolmaterial] field type '%s' not supported.\n", kvp.value.type.cStr() );
				continue;
			}

			kvp.key->setValue( pData, workingMemory );
		} 

		writer.openDataSection( 0u, AllocatorType_MainMemory );
		const ReferenceKey key = writer.addDataPoint();

		uint currentOffset = 0u;
		for (uint i = 0u; i < references.getCount(); ++i)
		{
			const MaterialDataReference& reference = references[ i ];

			writer.writeData( addPtr( pData, currentOffset ), reference.dataOffset - currentOffset );
			writer.writeReference( &reference.referenceKey );

			currentOffset += reference.dataOffset + 8u;
		}
		writer.writeData( addPtr( pData, currentOffset ), m_pDataType->getSize() - currentOffset );
		
		writer.closeDataSection();

		m_pDataType->disposeInstance( pData );

		return key;
	}

}