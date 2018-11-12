#pragma once

#include "tiki/graphics/graphics_state_object.hpp"

#include "tiki/graphics/color.hpp"

#if TIKI_ENABLED( TIKI_GRAPHICS_D3D11 )
#	include "../../../source/win_d3d11/graphics_states_d3d11.hpp"
#elif TIKI_ENABLED( TIKI_GRAPHICS_D3D12 )
#	include "../../../source/win_d3d12/graphics_states_d3d12.hpp"
#elif  TIKI_ENABLED( TIKI_GRAPHICS_VULKAN )
#	include "../../../source/global_vulkan/graphics_states_vulkan.hpp"
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

		bool				isCreated() const;

	private: // friends

		bool				create( GraphicsSystem& graphicsSystem, const SamplerStateParamters& creationParamter );
		void				dispose( GraphicsSystem& graphicsSystem );

	private:

		SamplerStatePlatformData	m_platformData;
	};
}
