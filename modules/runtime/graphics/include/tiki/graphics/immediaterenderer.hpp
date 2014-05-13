#pragma once
#ifndef TIKI_IMMEDIATERENDERER_HPP__INCLUDED
#define TIKI_IMMEDIATERENDERER_HPP__INCLUDED

#include "tiki/base/sizedarray.hpp"
#include "tiki/base/string.hpp"
#include "tiki/base/structs.hpp"
#include "tiki/base/types.hpp"
#include "tiki/graphics/constantbuffer.hpp"
#include "tiki/graphics/vertexbuffer.hpp"
#include "tiki/graphics/color.hpp"
#include "tiki/graphics/immediaterenderer_shader.hpp"

namespace tiki
{
	class BlendState;
	class DepthStencilState;
	class Font;
	class GraphicsContext;
	class GraphicsSystem;
	class Projection;
	class RasterizerState;
	class RenderTarget;
	class ResourceManager;
	class SamplerState;
	class ShaderSet;
	class TextureData;
	class VertexFormat;
	class VertexInputBinding;
	struct Rectangle; 
	struct Vector2;

	enum ImmediateBlendState
	{
		ImmediateBlendState_None,
		ImmediateBlendState_Add,

		ImmediateBlendState_Count
	};

	enum ImmediateDepthState
	{
		ImmediateDepthState_TestOffWriteOff,
		ImmediateDepthState_TestOnWriteOn,
		ImmediateDepthState_TestOnWriteOff,

		ImmediateDepthState_Count
	};

	

	class ImmediateRenderer
	{
		TIKI_NONCOPYABLE_CLASS( ImmediateRenderer );

	public:

							ImmediateRenderer();
							~ImmediateRenderer();
		
		bool				create( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager );
		void				dispose( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager );

		void				beginRendering( GraphicsContext& graphicsContext );
		void				endRendering();

		void				beginRenderPass( const RenderTarget* pRenderTarget = nullptr, const Projection* pProjection = nullptr );
		void				endRenderPass();

		void				setBlendState( ImmediateBlendState blendState );
		void				setDepthState( ImmediateDepthState depthState );

		void				drawRectangle( const Rectangle& dest, Color color = TIKI_COLOR_WHITE );
		void				drawTexturedRectangle( const TextureData& texture, const Rectangle& dest, Color color = TIKI_COLOR_WHITE );
		void				drawTexturedRectangle( const TextureData& texture, const Rectangle& dest, const Rectangle& src, Color color = TIKI_COLOR_WHITE );
		void				drawText( const Vector2& position, const Font& font, const char* pText, Color color = TIKI_COLOR_WHITE );

	private:

		struct ImmediateVertex
		{
			float3	position;
			uint16	u;
			uint16	v;
			Color	color;
		};

		GraphicsContext*					m_pContext;

		const ShaderSet*					m_pShaderSet;
		
		const BlendState*					m_pBlendState[ ImmediateBlendState_Count ];
		const DepthStencilState*			m_pDepthStencilState[ ImmediateDepthState_Count ];
		const RasterizerState*				m_pRasterizerState;
		const SamplerState*					m_pSamplerState;
		const VertexFormat*					m_pVertexFormat;
		const VertexInputBinding*			m_pVertexInputBinding;

		ConstantBuffer						m_vertexConstantBuffer;

		void						setState();

	};
}

#endif // TIKI_IMMEDIATERENDERER_HPP__INCLUDED
