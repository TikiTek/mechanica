
#include "tiki/graphics/samplerstate.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/crc32.hpp"
#include "tiki/base/functions.hpp"
#include "tiki/graphics/graphicssystem.hpp"

#include "graphicssystem_internal_opengl4.hpp"

namespace tiki
{
	static const GLenum s_aAddressModeMapping[] =
	{
		0x2901,		//GL_REPEAT​,			// AddressMode_Wrap
		GL_MIRRORED_REPEAT,	// AddressMode_Mirror
		0x812F,		//GL_CLAMP_TO_EDGE​,	// AddressMode_Clamp
		0x812D,		//GL_CLAMP_TO_BORDER​	// AddressMode_Border
	};
	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aAddressModeMapping ) == AddressMode_Count );

	static bool getFilter( GLenum& targetMinFilter, GLenum& targetMagFilter, FilterMode minMag, FilterMode mip )
	{
		switch ( minMag )
		{
		case FilterMode_Anisotropic:
			TIKI_ASSERT( mip == FilterMode_Anisotropic );
			
			targetMinFilter = GL_LINEAR;
			targetMagFilter = GL_LINEAR;
			break;

		case FilterMode_Linear:
			{
				targetMagFilter = GL_LINEAR;

				switch ( mip )
				{
				case FilterMode_Linear:
					targetMinFilter = GL_LINEAR_MIPMAP_LINEAR;
					break;

				case FilterMode_Nearest:
					targetMinFilter = GL_LINEAR_MIPMAP_NEAREST;
					break;

				default:
					return false;
				}
			}
			break;
		case FilterMode_Nearest:
			{
				targetMagFilter = GL_NEAREST;

				switch ( mip )
				{
				case FilterMode_Linear:
					targetMinFilter = GL_NEAREST_MIPMAP_LINEAR;
					break;

				case  FilterMode_Nearest:
					targetMinFilter = GL_NEAREST_MIPMAP_NEAREST;
					break;

				default:
					return false;
				}
			}
			break;

		default:
			return false;
		}

		return true;
	}

	bool SamplerState::isCreated() const
	{
		return m_platformData.samplerId != GL_INVALID_ENUM;
	}

	bool SamplerState::create( GraphicsSystem& graphicsSystem, const SamplerStateParamters& creationParamter )
	{
		glGenSamplers( 1u, &m_platformData.samplerId );

		GLenum minFilter;
		GLenum magFilter;
		if ( !getFilter( minFilter, magFilter, creationParamter.magFilter, creationParamter.mipFilter ) )
		{
			dispose( graphicsSystem );
			return false;
		}

		glSamplerParameteri( m_platformData.samplerId, GL_TEXTURE_MIN_FILTER,			minFilter );
		glSamplerParameteri( m_platformData.samplerId, GL_TEXTURE_MAG_FILTER,			magFilter );
		glSamplerParameteri( m_platformData.samplerId, GL_TEXTURE_WRAP_S,				s_aAddressModeMapping[ creationParamter.addressU ] );
		glSamplerParameteri( m_platformData.samplerId, GL_TEXTURE_WRAP_T,				s_aAddressModeMapping[ creationParamter.addressV ] );
		glSamplerParameteri( m_platformData.samplerId, GL_TEXTURE_WRAP_R,				s_aAddressModeMapping[ creationParamter.addressW ] );
		glSamplerParameteri( m_platformData.samplerId, GL_TEXTURE_BORDER_COLOR,			creationParamter.borderColor );
		glSamplerParameterf( m_platformData.samplerId, GL_TEXTURE_MAX_ANISOTROPY_EXT,	(float)creationParamter.maxAnisotropy );
		
		return true;
	}

	void SamplerState::dispose( GraphicsSystem& graphicsSystem )
	{
		if ( m_platformData.samplerId != GL_INVALID_ENUM )
		{
			glDeleteSamplers( 1u, &m_platformData.samplerId );
			m_platformData.samplerId = GL_INVALID_ENUM;
		}

		GraphicsStateObject::dispose();
	}
}