
#include "tiki/graphicsresources/font.hpp"

namespace tiki
{
	Font::Font()
	{		
	}

	Font::~Font()
	{
	}

	bool Font::initialize( GraphicsSystem& graphicsSystem, const void* pInitData )
	{
		TIKI_ASSERT( pInitData != nullptr );
				
		const uint32* pData = (uint32*)pInitData;
		const uint32 textureSize = *pData++;
		
		const TextureDescription* pDescription = (const TextureDescription*)pData;

		pData = addPtr( pData, sizeof( TextureDescription ) );
		bool success = m_textureData.create( graphicsSystem, *pDescription, pData );

		pData = addPtrCast< uint32 >( pInitData, textureSize + sizeof( uint32 ) );
		uint32 charCount = *pData++;
		m_chars.create( (const FontChar*)pData, charCount );

		return success;
	}

	void Font::dispose()
	{
		m_chars.dispose();
		m_textureData.dispose();
	}

	void Font::fillVertices( FontChar* pChars, size_t vertexStride, size_t repeateChar, cstring text, size_t capacity ) const
	{
		for (size_t i = 0u; i < capacity; ++i)
		{
			TIKI_ASSERT( text[ i ] != '\0' );

			for (size_t j = 0u; j < repeateChar; ++j)
			{
				pChars[ 0u ]	= m_chars[ text[ i ] ];
				pChars			= addPtr( pChars, vertexStride );
			}
		}
	}
}
