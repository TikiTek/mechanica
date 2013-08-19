#include "tiki/graphics/material.hpp"

#include "tiki/base/memory.hpp"
#include "tiki/base/assert.hpp"
#include "tiki/base/string.hpp"
#include "tiki/base/types.hpp"

#include "tiki/graphics/shader.hpp"
#include "tiki/graphics/texture.hpp"

#include "tiki/framework/framework.hpp"
#include "tiki/resource/resourcemanager.hpp"


namespace tiki
{
	Material::Material() 
		:m_pPixelShader( nullptr ), m_pVertexShader( nullptr )
	{

	}

	Material::~Material()
	{
		TIKI_ASSERT( !m_pPixelShader );
		TIKI_ASSERT( !m_pVertexShader );
	}

	bool Material::initialize( const void* pData )
	{
		ResourceManager& content = framework::getResourceManager();

		uint stringLength = *(uint*)pData;
		pData = addPtr( pData, sizeof(uint) );

		string vertexShader( (char*)pData, stringLength );
		m_pVertexShader = content.loadResource< Shader >( vertexShader );
		TIKI_ASSERT( m_pVertexShader );

		pData = addPtr( pData, stringLength );

		stringLength = *(uint*)pData;
		pData = addPtr( pData, sizeof(uint) );

		string pixelShader( (char*)pData, stringLength );
		m_pPixelShader = content.loadResource< Shader >( pixelShader );
		TIKI_ASSERT( m_pPixelShader );
		pData = addPtr( pData, stringLength );

		const uint count = *(uint*)pData;
		pData = addPtr( pData, sizeof(uint) );

		if( count )
		{
			m_textures.create( count );
			for( uint i = 0; i < count; ++i )
			{
				IATextureResource& iatr = m_textures[ i ];

				stringLength = *(uint*)pData;
				pData = addPtr( pData, sizeof(uint) );
				string textureName( (char*)pData, stringLength );
				pData = addPtr( pData, stringLength );

				iatr.m_pTexture = content.loadResource< Texture >( textureName );
				TIKI_ASSERT( iatr.m_pTexture );

				const uint slot = *(uint*)pData;
				pData = addPtr( pData, sizeof(uint) );
				iatr.m_slot = slot;
			}
		}
		return true;
	}

	void Material::dispose()
	{
		ResourceManager& content = framework::getResourceManager();

		content.unloadResource( m_pVertexShader );
		content.unloadResource( m_pPixelShader );

		m_pVertexShader = nullptr;
		m_pPixelShader	= nullptr;

		for( uint i = 0; i < m_textures.getCount(); ++i )
		{
			IATextureResource& iatr = m_textures[ i ];

			content.unloadResource( iatr.m_pTexture );
			iatr.m_pTexture = nullptr;
		}
		m_textures.dispose();
	}
}


