#pragma once
#ifndef __TIKI_RASTERIZERSTATE_HPP_INCLUDED__
#define __TIKI_RASTERIZERSTATE_HPP_INCLUDED__

#include "tiki/graphics/graphicsstateobject.hpp"

#if TIKI_ENABLED( TIKI_GRAPHICS_D3D11 )
#	include "../../../source/win_d3d11/graphicsstates_d3d11.hpp"
#elif  TIKI_ENABLED( TIKI_GRAPHICS_OPENGL4 )
#	include "../../../source/global_opengl4/graphicsstates_opengl4.hpp"
#else
#	error Platform not supported
#endif

namespace tiki
{
	enum FillMode
	{
		FillMode_Wireframe,
		FillMode_Solid,

		FillMode_Count
	};

	enum CullMode
	{
		CullMode_None,
		CullMode_Front,
		CullMode_Back,

		CullMode_Count
	};

	enum WindingOrder
	{
		WindingOrder_Clockwise,
		WindingOrder_CounterClockwise,

		WindingOrder_Count
	};

	struct RasterizerStateParamters
	{
		RasterizerStateParamters()
		{
			fillMode		= FillMode_Solid;
			cullMode		= CullMode_Back;
			windingOrder	= WindingOrder_Clockwise;
		}

		FillMode		fillMode;
		CullMode		cullMode;
		WindingOrder	windingOrder;
	};

	class RasterizerState : public GraphicsStateObject
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( RasterizerState );
		friend class GraphicsContext;
		friend class GraphicsSystem;

	public:

		bool						isCreated() const;

	private: // friends

		bool						create( GraphicsSystem& graphicsSystem, const RasterizerStateParamters& creationParamter );
		void						dispose( GraphicsSystem& graphicsSystem );

	private:

		RasterizerStatePlatformData	m_platformData;
	
	};
}

#endif // __TIKI_RASTERIZERSTATE_HPP_INCLUDED__
