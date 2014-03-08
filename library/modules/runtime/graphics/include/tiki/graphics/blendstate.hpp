#pragma once
#ifndef __TIKI_BLENDSTATE_HPP_INCLUDED__
#define __TIKI_BLENDSTATE_HPP_INCLUDED__

#include "tiki/graphicsbase/graphicsstateobject.hpp"

#if TIKI_ENABLED( TIKI_GRAPHICS_D3D11 )
#	include "win_d3d11/graphicsstatesplatformdata_d3d11.hpp"
#elif  TIKI_ENABLED( TIKI_GRAPHICS_OPENGL4 )
#	include "global_opengl4/graphicsstatesplatformdata_opengl4.hpp"
#else
#	error Platform not supported
#endif

namespace tiki
{
	enum Blend
	{
		Blend_Zero,
		Blend_One,
		Blend_SourceColor,
		Blend_InverseSourceColor,
		Blend_SourceAlpha,
		Blend_InverseSourceAlpha,
		Blend_DestinationColor,
		Blend_InverseDestinationColor,
		Blend_DestinationAlpha,
		Blend_InverseDestinationAlpha,

		Blend_Count
	};

	enum BlendOperation
	{
		BlendOperation_Add,
		BlendOperation_Subtract,
		BlendOperation_Min,
		BlendOperation_Max,

		BlendOperation_Count
	};

	enum ColorWriteMask
	{
		ColorWriteMask_Red		= 1u << 0u,
		ColorWriteMask_Green	= 1u << 1u,
		ColorWriteMask_Blue		= 1u << 2u,
		ColorWriteMask_Alpha	= 1u << 3u,

		ColorWriteMask_All		= ( ColorWriteMask_Red | ColorWriteMask_Green | ColorWriteMask_Blue | ColorWriteMask_Alpha )
	};

	struct BlendStateParamters
	{
		BlendStateParamters()
		{
			blendEnabled		= false;

			sourceBlend			= Blend_One;
			destinationBlend	= Blend_Zero;
			operation			= BlendOperation_Add;

			colorWriteMask		= ColorWriteMask_All;
		}

		bool			blendEnabled;

		Blend			sourceBlend;
		Blend			destinationBlend;
		BlendOperation	operation;

		ColorWriteMask	colorWriteMask;

		//D3D11_BLEND SrcBlendAlpha;
		//D3D11_BLEND DestBlendAlpha;
		//D3D11_BLEND_OP BlendOpAlpha;
	};

	class BlendState : public GraphicsStateObject
	{
		TIKI_NONCOPYABLE_CLASS( BlendState );
		friend class GraphicsContext;
		friend class GraphicsSystem;

	public:

		bool					isCreated() const;

	private: // friends

		bool					create( GraphicsSystem& graphicsSystem, const BlendStateParamters& creationParamter );
		void					dispose();

	private:

		BlendStatePlatformData	m_platformData;
	
	};
}

#endif // __TIKI_BLENDSTATE_HPP_INCLUDED__
