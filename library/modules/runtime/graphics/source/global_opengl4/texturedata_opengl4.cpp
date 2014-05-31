
#include "tiki/graphics/texturedata.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/functions.hpp"
#include "tiki/base/memory.hpp"

#include "graphicssystem_internal_opengl4.hpp"

namespace tiki
{
	GLenum graphics::getGlFormat( PixelFormat pixelFormat )
	{
		TIKI_ASSERT( pixelFormat < PixelFormat_Count );

		static GLenum s_formatMapping[] =
		{
			
			GL_R8,					// PixelFormat_R8,
			GL_SRGB8_ALPHA8,		// PixelFormat_R8G8B8A8
			GL_SRGB8,				// PixelFormat_R8G8B8A8_Gamma
			GL_RGBA16F,				// PixelFormat_R16G16B16A16_Float
			GL_R32F,				// PixelFormat_R32_Float
			GL_RGB32F,				// PixelFormat_R32G32B32_Float
			GL_RGBA32F,				// PixelFormat_R32G32B32A32_Float
			GL_DEPTH24_STENCIL8		// PixelFormat_Depth24Stencil8
		};
		TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_formatMapping ) == PixelFormat_Count );

		return s_formatMapping[ pixelFormat ];
	}

	GLenum graphics::getGlChannelType( PixelFormat pixelFormat )
	{
		TIKI_ASSERT( pixelFormat < PixelFormat_Count );

		static GLenum s_typeMapping[] =
		{

			GL_UNSIGNED_BYTE,		// PixelFormat_R8,
			GL_UNSIGNED_BYTE,		// PixelFormat_R8G8B8A8
			GL_UNSIGNED_BYTE,		// PixelFormat_R8G8B8A8_Gamma
			GL_2_BYTES,				// PixelFormat_R16G16B16A16_Float
			GL_FLOAT,				// PixelFormat_R32_Float
			GL_FLOAT,				// PixelFormat_R32G32B32_Float
			GL_FLOAT,				// PixelFormat_R32G32B32A32_Float
			GL_UNSIGNED_INT			// PixelFormat_Depth24Stencil8
		};
		TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_typeMapping ) == PixelFormat_Count );

		return s_typeMapping[ pixelFormat ];
	}

	static GLenum getGlTextureType( TextureType type )
	{
		TIKI_ASSERT( type < TextureType_Count );

		static GLenum s_typeMapping[] =
		{
			GL_TEXTURE_1D,			// TextureType_1d
			GL_TEXTURE_2D,			// TextureType_2d
			GL_TEXTURE_3D,			// TextureType_3d
			GL_TEXTURE_CUBE_MAP,	// TextureType_Cube
		};
		TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_typeMapping ) == TextureType_Count );

		return s_typeMapping[ type ];
	}

	static void initTextureLevelData( TextureType type, GLint level, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum channelType, const void* pData )
	{
		switch ( type )
		{
		case TextureType_1d:
			glTexImage1D( GL_TEXTURE_1D, level, format, width, 0, format, channelType, pData );
			break;

		case TextureType_2d:
			glTexImage2D( GL_TEXTURE_2D, level, format, width, height, 0, format, channelType, pData );
			break;

		case TextureType_3d:
			glTexImage3D( GL_TEXTURE_3D, level, format, width, height, depth, 0, format, channelType, pData );
			break;

		case TextureType_Cube:
			TIKI_TRACE_ERROR( "[graphics] Not jet implemented.\n" );
			break;
		}		
	}

	//static UINT getD3dFlags( TextureFlags flags )
	//{
	//	UINT result = 0u;

	//	if ( isBitSet( flags, TextureFlags_RenderTarget ) )
	//	{
	//		result |= D3D11_BIND_RENDER_TARGET;
	//	}

	//	if ( isBitSet( flags, TextureFlags_DepthStencil ) )
	//	{
	//		result |= D3D11_BIND_DEPTH_STENCIL;
	//	}

	//	if ( isBitSet( flags, TextureFlags_ShaderInput ) )
	//	{
	//		result |= D3D11_BIND_SHADER_RESOURCE;
	//	}

	//	return result;
	//}

	TextureData::TextureData()
	{
	}

	TextureData::~TextureData()
	{
		//TIKI_ASSERT( m_platformData.pResource == nullptr );
		//TIKI_ASSERT( m_platformData.pShaderView == nullptr );
	}

	bool TextureData::create( GraphicsSystem& graphicsSystem, const TextureDescription& description, const void* pTextureData /*= nullptr*/ )
	{
		//TIKI_ASSERT( m_platformData.pResource == nullptr );
		//TIKI_ASSERT( m_platformData.pShaderView == nullptr );
		TIKI_ASSERT( description.type != TextureType_Cube ); // cube textures need to be implemented

		m_description = description;

		const GLenum glFormat = graphics::getGlFormat( (PixelFormat)description.format );
		const GLenum glChannelType = graphics::getGlChannelType( (PixelFormat)description.format );
		const GLenum glTextureType = getGlTextureType( (TextureType)description.type );

		glGenTextures( 1, &m_platformData.textureId );
		glBindTexture( glTextureType, m_platformData.textureId );


		if ( pTextureData != nullptr )
		{
			const uint bytesPerPixel = getBitsPerPixel( (PixelFormat)description.format ) / 8u;

			uint width	= description.width;
			uint height	= description.height;
			uint depth	= TIKI_MAX( description.depth, 1u );
			const uint8* pLevelData	= static_cast< const uint8* >( pTextureData );

			for (uint mipLevel = 0u; mipLevel <= description.mipCount; ++mipLevel)
			{
				const uint rowPitch		= width * bytesPerPixel;
				const uint depthPitch	= rowPitch * height;

				initTextureLevelData(
					(TextureType)description.type,
					mipLevel,
					width,
					height,
					depth,
					glFormat,
					glChannelType,
					pLevelData
				);
				
				pLevelData	+= depthPitch * depth;
				width		/= 2u;
				height		/= 2u;
				depth		= TIKI_MAX( depth / 2u, 1u );
			} 
		}
		else
		{
			uint width	= description.width;
			uint height	= description.height;
			uint depth	= TIKI_MAX( description.depth, 1u );

			for (uint mipLevel = 0u; mipLevel <= description.mipCount; ++mipLevel)
			{
				initTextureLevelData(
					(TextureType)description.type,
					mipLevel,
					width,
					height,
					depth,
					glFormat,
					glChannelType,
					0
				);

				width		/= 2u;
				height		/= 2u;
				depth		= TIKI_MAX( depth / 2u, 1u );
			}
		}

		glBindTexture( glTextureType, 0u );

		return true;
	}

	void TextureData::dispose( GraphicsSystem& graphicsSystem )
	{
		if ( m_platformData.textureId != 0u )
		{
			glDeleteTextures( 1, &m_platformData.textureId );
			m_platformData.textureId = 0u;
		}
	}
}