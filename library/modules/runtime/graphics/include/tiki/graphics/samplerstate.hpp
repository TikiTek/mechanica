#ifndef TIKI_SAMPLERSTATE_HPP__
#define TIKI_SAMPLERSTATE_HPP__

#include "tiki/graphics/graphicsstateobject.hpp"

#include "tiki/graphics/color.hpp"

#if TIKI_ENABLED( TIKI_GRAPHICS_D3D11 )
#	include "win_d3d11/graphicsstatesplatformdata_d3d11.hpp"
#elif  TIKI_ENABLED( TIKI_GRAPHICS_OPENGL4 )
#	include "global_opengl4/graphicsstatesplatformdata_opengl4.hpp"
#else
#	error Platform not supported
#endif

namespace tiki
{
	enum AddressMode
	{
		AddressMode_Wrap,
		AddressMode_Mirror,
		AddressMode_Clamp,
		AddressMode_Border,

		AddressMode_Count
	};

	enum FilterMode
	{
		FilterMode_Nearest,
		FilterMode_Linear,
		FilterMode_Anisotropic,

		FilterMode_Count
	};

	struct SamplerStateParamters
	{
		AddressMode addressU;
		AddressMode addressV;
		AddressMode addressW;
		
		FilterMode	magFilter;
		FilterMode	mipFilter;
		
		uint		maxAnisotropy;
		Color		borderColor;

		SamplerStateParamters()
		{
			addressU		= AddressMode_Wrap;
			addressV		= AddressMode_Wrap;
			addressW		= AddressMode_Wrap;
			
			magFilter		= FilterMode_Linear;
			mipFilter		= FilterMode_Nearest;

			maxAnisotropy	= 1u;
			borderColor		= TIKI_COLOR_BLACK;
		}
	};

	class SamplerState : public GraphicsStateObject
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( SamplerState );
		friend class GraphicsContext;
		friend class GraphicsSystem;

	public:

		bool						isCreated() const;

	private: // friends

		bool						create( GraphicsSystem& graphicsSystem, const SamplerStateParamters& creationParamter );
		void						dispose();

	private:

		SamplerStatePlatformData	m_platformData;

	};
}

#endif // TIKI_SAMPLERSTATE_HPP__
