#include "tiki/materialconverter/toolmaterial.hpp"

#include "tiki/base/reflection.hpp"
#include "tiki/converterbase/resourcewriter.hpp"
#include "tiki/toolbase/tikixml.hpp"

namespace tiki
{
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

		const XmlElement* pValueNode = xml.findFirstChild( "field", pRootNode );
		while ( pValueNode )
		{
			const XmlAttribute* pNameAtt = xml.findAttributeByName( "name", pValueNode );
			if ( pNameAtt == nullptr )
			{
				TIKI_TRACE_ERROR( "[materialconverter] Could not find 'name' attribute of field.\n" );
				continue;
			}

			const XmlAttribute* pTypeAtt = xml.findAttributeByName( "type", pValueNode );
			if ( pTypeAtt == nullptr )
			{
				TIKI_TRACE_ERROR( "[materialconverter] Field with name '%s' has no 'type' attribute.\n", pNameAtt->content );
				continue;
			}

			const reflection::FieldMember* pField = m_pDataType->getFieldByName( pNameAtt->content );
			if ( pField == nullptr )
			{
				TIKI_TRACE_ERROR( "[materialconverter] Field with name '%s' is not a member of Type: '%s'.\n", pNameAtt->content, m_pDataType->getName().cStr() );
				continue;
			}

			const string typeString = pTypeAtt->content;
			if ( typeString == "resource" )
			{

			}

			pValueNode = xml.findNext( "field", pValueNode );
		}

		xml.dispose();
		return true;
	}

	void ToolMaterial::dispose()
	{
		m_effectData.dispose();
	}

	bool ToolMaterial::writeResource( ResourceWriter& writer ) const
	{
		return false;
	}

}