
#include "tiki/graphics/texturedata.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/functions.hpp"
#include "tiki/base/memory.hpp"

#include "graphicssystem_internal_opengl4.hpp"

namespace tiki
{
	static const GLenum s_aInternalFormatMapping[] =
	{
		GL_R8,					// PixelFormat_R8,
		GL_RGBA8,				// PixelFormat_R8G8B8A8
		GL_SRGB8_ALPHA8,		// PixelFormat_R8G8B8A8_Gamma
		GL_RGBA16F,				// PixelFormat_R16G16B16A16_Float
		GL_R32F,				// PixelFormat_R32_Float
		GL_RGB32F,				// PixelFormat_R32G32B32_Float
		GL_RGBA32F,				// PixelFormat_R32G32B32A32_Float
		GL_DEPTH24_STENCIL8		// PixelFormat_Depth24Stencil8
	};
	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aInternalFormatMapping ) == PixelFormat_Count );

	static const GLenum s_aFormatMapping[] =
	{
		GL_RED,					// PixelFormat_R8,
		GL_RGBA,				// PixelFormat_R8G8B8A8
		GL_RGBA,				// PixelFormat_R8G8B8A8_Gamma
		GL_RGBA,				// PixelFormat_R16G16B16A16_Float
		GL_RED,					// PixelFormat_R32_Float
		GL_RGB,					// PixelFormat_R32G32B32_Float
		GL_RGBA,				// PixelFormat_R32G32B32A32_Float
		GL_DEPTH_STENCIL		// PixelFormat_Depth24Stencil8
	};
	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aFormatMapping ) == PixelFormat_Count );

	static const GLenum s_aChannelTypeMapping[] =
	{
		GL_UNSIGNED_BYTE,		// PixelFormat_R8,
		GL_UNSIGNED_BYTE,		// PixelFormat_R8G8B8A8
		GL_UNSIGNED_BYTE,		// PixelFormat_R8G8B8A8_Gamma
		GL_SHORT,				// PixelFormat_R16G16B16A16_Float
		GL_FLOAT,				// PixelFormat_R32_Float
		GL_FLOAT,				// PixelFormat_R32G32B32_Float
		GL_FLOAT,				// PixelFormat_R32G32B32A32_Float
		GL_UNSIGNED_INT_24_8	// PixelFormat_Depth24Stencil8
	};
	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aChannelTypeMapping ) == PixelFormat_Count );

	static const GLenum s_aTypeMapping[] =
	{
		GL_TEXTURE_1D,			// TextureType_1d
		GL_TEXTURE_2D,			// TextureType_2d
		GL_TEXTURE_3D,			// TextureType_3d
		GL_TEXTURE_CUBE_MAP,	// TextureType_Cube
	};
	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aTypeMapping ) == TextureType_Count );

	GLenum graphics::getGlFormat( PixelFormat pixelFormat )
	{
		TIKI_ASSERT( pixelFormat < PixelFormat_Count );
		return s_aInternalFormatMapping[ pixelFormat ];
	}

	GLenum graphics::getGlChannelType( PixelFormat pixelFormat )
	{
		TIKI_ASSERT( pixelFormat < PixelFormat_Count );
		return s_aChannelTypeMapping[ pixelFormat ];
	}

	GLenum graphics::getGlTextureType( TextureType type )
	{
		TIKI_ASSERT( type < TextureType_Count );
		return s_aTypeMapping[ type ];
	}

	static bool initTextureLevelData( TextureType type, GLint level, GLsizei width, GLsizei height, GLsizei depth, GLenum internalFormat, GLenum format, GLenum channelType, const void* pData )
	{
		switch ( type )
		{
		case TextureType_1d:
			glTexImage1D( GL_TEXTURE_1D, level, internalFormat, width, 0, format, channelType, pData );
			break;

		case TextureType_2d:
			glTexImage2D( GL_TEXTURE_2D, level, internalFormat, width, height, 0, format, channelType, pData );
			break;

		case TextureType_3d:
			glTexImage3D( GL_TEXTURE_3D, level, internalFormat, width, height, depth, 0, format, channelType, pData );
			break;

		case TextureType_Cube:
			TIKI_TRACE_ERROR( "[graphics] Not jet implemented.\n" );
			break;

		default:
			TIKI_BREAK( "[graphics] TextureType not supported.\n" );
			break;
		}

		return graphics::checkError();
	}

	TextureData::TextureData()
	{
		m_platformData.textureId = GL_INVALID_ENUM;
	}

	TextureData::~TextureData()
	{
		TIKI_ASSERT( m_platformData.textureId == GL_INVALID_ENUM );
	}

	bool TextureData::create( GraphicsSystem& graphicsSystem, const TextureDescription& description, const void* pTextureData /*= nullptr*/ )
	{
		TIKI_ASSERT( m_platformData.textureId == GL_INVALID_ENUM );
		TIKI_ASSERT( description.type != TextureType_Cube ); // cube textures need to be implemented

		m_description				= description;
		m_platformData.textureType	= graphics::getGlTextureType( (TextureType)description.type );

		const GLenum glFormat		= graphics::getGlFormat( (PixelFormat)description.format );
		const GLenum glChannelType	= graphics::getGlChannelType( (PixelFormat)description.format );

		glGenTextures( 1, &m_platformData.textureId );
		glBindTexture( m_platformData.textureType, m_platformData.textureId );

		if ( description.format != PixelFormat_Depth24Stencil8 )
		{
			glTexParameteri( m_platformData.textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			glTexParameteri( m_platformData.textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
			glTexParameteri( m_platformData.textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
			glTexParameteri( m_platformData.textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		}
		
		bool success = true;
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

				success &= initTextureLevelData(
					(TextureType)description.type,
					mipLevel,
					width,
					height,
					depth,
					glFormat,
					s_aFormatMapping[ description.format ],
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
				success &= initTextureLevelData(
					(TextureType)description.type,
					mipLevel,
					width,
					height,
					depth,
					glFormat,
					s_aFormatMapping[ description.format ],
					glChannelType,
					0
				);

				width		/= 2u;
				height		/= 2u;
				depth		= TIKI_MAX( depth / 2u, 1u );
			}
		}

		glBindTexture( m_platformData.textureType, 0u );

		if ( !success )
		{
			dispose( graphicsSystem );
		}

		return success;
	}

	void TextureData::dispose( GraphicsSystem& graphicsSystem )
	{
		if ( m_platformData.textureId != GL_INVALID_ENUM )
		{
			glDeleteTextures( 1, &m_platformData.textureId );
			m_platformData.textureId = GL_INVALID_ENUM;
		}
	}
}