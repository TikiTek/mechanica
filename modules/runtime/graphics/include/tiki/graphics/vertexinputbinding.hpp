#pragma once
#ifndef TIKI_VERTEXINPUTBINDING_HPP
#define TIKI_VERTEXINPUTBINDING_HPP

#include "tiki/base/types.hpp"
#include "tiki/graphics/graphissystemlimits.hpp"
#include "tiki/graphics/graphicsstateobject.hpp"

#if TIKI_ENABLED( TIKI_GRAPHICS_D3D11 )
#	include "win_d3d11/graphicsstatesplatformdata_d3d11.hpp"
#elif TIKI_ENABLED( TIKI_GRAPHICS_OPENGL4 )
#	include "global_opengl4/graphicsstatesplatformdata_opengl4.hpp"
#else
#	error Platform not implemented
#endif

namespace tiki
{
	class Shader;
	class VertexFormat;

	struct VertexInputBindingParameters
	{
		const VertexFormat*	pVertexFormat;
		const Shader*		pShader;
	};

	class VertexInputBinding : public GraphicsStateObject
	{
		TIKI_NONCOPYABLE_CLASS( VertexInputBinding );
		friend class GraphicsContext;
		friend class GraphicsSystem;

	public:

										VertexInputBinding();
		virtual							~VertexInputBinding();

		bool							isCreated() const { return m_pShader != nullptr; }

		const VertexFormat*				getVertexFormat() const { return m_pVertexFormat; }

	private: // friends

		bool							create( GraphicsSystem& graphicsSystem, const VertexInputBindingParameters& creationParameters );
		void							dispose();

	private:

		const VertexFormat*				m_pVertexFormat;
		const Shader*					m_pShader;
		VertexInputBindingPlatformData	m_platformData;

	};
}

#endif // TIKI_VERTEXINPUTBINDING_HPP