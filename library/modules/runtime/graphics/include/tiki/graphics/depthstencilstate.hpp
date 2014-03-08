#pragma once
#ifndef __TIKI_DEPTHSTENCILSTATE_HPP_INCLUDED__
#define __TIKI_DEPTHSTENCILSTATE_HPP_INCLUDED__

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
	enum ComparsionFunction
	{
		ComparsionFunction_Never,
		ComparsionFunction_Less,
		ComparsionFunction_Equal,
		ComparsionFunction_LessOrEqual,
		ComparsionFunction_Greater,
		ComparsionFunction_NotEqual,
		ComparsionFunction_GreaterOrEqual,
		ComparsionFunction_Always,

		ComparsionFunction_Count
	};

	enum StencilOperation
	{
		StencilOperation_Keep,
		StencilOperation_Zero,
		StencilOperation_Replace,
		StencilOperation_Increase,
		StencilOperation_Decrease,

		StencilOperation_Count
	};

	struct StencilParameters
	{
		StencilParameters()
		{
			stencilFunction			= ComparsionFunction_Never;

			stencilPassOperation	= StencilOperation_Keep;
			stencilFailOperation	= StencilOperation_Keep;
			depthFailOperation		= StencilOperation_Keep;
		}

		ComparsionFunction	stencilFunction;

		StencilOperation	stencilFailOperation;
		StencilOperation	stencilPassOperation;
		StencilOperation	depthFailOperation;
	};

	struct DepthStencilStateParamters
	{
		DepthStencilStateParamters()
		{
			depthEnabled		= false;
			depthFunction		= ComparsionFunction_Never;

			stencilEnabled		= false;
			stencilReadMask		= 0u;
			stencilWriteMask	= 0u;
			stencilReference	= 0u;
		}

		bool				depthEnabled;
		ComparsionFunction	depthFunction;

		bool				stencilEnabled;
		uint8				stencilReadMask;
		uint8				stencilWriteMask;
		uint8				stencilReference;

		StencilParameters	frontFace;
		StencilParameters	backFace;
	};

	class DepthStencilState : public GraphicsStateObject
	{
		TIKI_NONCOPYABLE_CLASS( DepthStencilState );
		friend class GraphicsContext;
		friend class GraphicsSystem;

	public:

		bool							isCreated() const;

	private: // friends

		bool							create( GraphicsSystem& graphicsSystem, const DepthStencilStateParamters& creationParamter );
		void							dispose();

	private:

		DepthStencilStatePlatformData	m_platformData;
	
	};
}

#endif // __TIKI_DEPTHSTENCILSTATE_HPP_INCLUDED__
