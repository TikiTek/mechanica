#include "tiki/materialconverter/toolmaterial.hpp"

#include "tiki/base/stringparse.hpp"

namespace tiki
{
	ToolMaterial::ToolMaterial()
	{

	}

	ToolMaterial::~ToolMaterial()
	{

	}

	bool ToolMaterial::create( const string& fileName )
	{
		m_xml.create( fileName );

		const XmlElement* pAsset = m_xml.findNodeByName( "tikiasset" );
		if( !pAsset ) { TIKI_ASSERT( false ); return false; }
			
		const XmlElement* pShader = m_xml.findFirstChild( "vertex", pAsset );
		if( !pShader ) { TIKI_ASSERT( false ); return false; }

		const XmlAttribute* pContent = m_xml.findAttributeByName( "shader", pShader );
		if( !pContent ) { TIKI_ASSERT( false ); return false; }
		m_vertexShader = pContent->content;

		pShader = m_xml.findFirstChild( "pixel", pAsset );
		if( !pShader ) { TIKI_ASSERT( false ); return false; }

		pContent = m_xml.findAttributeByName( "shader", pShader );
		if( !pContent ) { TIKI_ASSERT( false ); return false; }
		m_pixelShader = pContent->content;

		const XmlElement* pTexture = m_xml.findFirstChild( "texture", pAsset );
		while( pTexture )
		{
			const XmlAttribute* pFile = m_xml.findAttributeByName( "file", pTexture );
			const XmlAttribute* pSlot = m_xml.findAttributeByName( "slot", pTexture );

			TIKI_ASSERT( pFile );
			TIKI_ASSERT( pSlot );

			TextureAsset asset;
			asset.fileName = pFile->content;
			asset.slot = ParseString::parseUInt32( pSlot->content );
			m_assets.add( asset );

			pTexture = m_xml.findNext( "texture", pTexture );
		}

		return true;
	}

	void ToolMaterial::dispose()
	{
		m_xml.dispose();
	}
}