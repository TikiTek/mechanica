#pragma once
#ifndef TIKI_IMMEDIATERENDERER_HPP__INCLUDED
#define TIKI_IMMEDIATERENDERER_HPP__INCLUDED

#include "tiki/base/string.hpp"
#include "tiki/base/types.hpp"
#include "tiki/container/sizedarray.hpp"
#include "tiki/container/staticarray.hpp"
#include "tiki/graphics/color.hpp"
#include "tiki/graphics/constantbuffer.hpp"
#include "tiki/graphics/primitivetopologies.hpp"
#include "tiki/graphics/vertexbuffer.hpp"

#include "base.hpp"

namespace tiki
{
	class BlendState;
	class Camera;
	class DepthStencilState;
	class Font;
	class GraphicsContext;
	class GraphicsSystem;
	class RasterizerState;
	class RenderTarget;
	class ResourceManager;
	class SamplerState;
	class ShaderSet;
	class TextureData;
	class VertexFormat;
	class VertexInputBinding;
	struct AxisAlignedRectangle;
	struct ImmediateVertex;
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

	enum ImmediateShaderMode
	{
		ImmediateShaderMode_Texture,
		ImmediateShaderMode_Font,
		ImmediateShaderMode_Color,

		ImmediateShaderMode_Count
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

		void				beginRenderPass( const RenderTarget* pRenderTarget = nullptr, const Camera* pProjection = nullptr ) const;
		void				endRenderPass() const;

		void				setBlendState( ImmediateBlendState blendState ) const;
		void				setDepthState( ImmediateDepthState depthState ) const;
		void				setPrimitiveTopology( PrimitiveTopology topology ) const;
		void				setShaderMode( ImmediateShaderMode shaderMode) const;

		void				drawRectangle( const AxisAlignedRectangle& destinationCoodinates, Color color = TIKI_COLOR_WHITE ) const;
		void				drawTexturedRectangle( const TextureData& texture, const AxisAlignedRectangle& destinationCoodinates, Color color = TIKI_COLOR_WHITE ) const;
		void				drawTexturedRectangle( const TextureData& texture, const AxisAlignedRectangle& destinationCoodinates, const AxisAlignedRectangle& sourceCoodinates, Color color = TIKI_COLOR_WHITE ) const;
		void				drawText( const Vector2& position, const Font& font, const char* pText, Color color = TIKI_COLOR_WHITE ) const;

		void				beginImmediateGeometry( StaticArray< ImmediateVertex >& vertices, uint capacity ) const;
		void				endImmediateGeometry( StaticArray< ImmediateVertex >& vertices ) const;

	private:

		GraphicsContext*			m_pContext;

		const ShaderSet*			m_pShaderSet;
		
		const BlendState*			m_pBlendState[ ImmediateBlendState_Count ];
		const DepthStencilState*	m_pDepthStencilState[ ImmediateDepthState_Count ];
		const RasterizerState*		m_pRasterizerState;
		const SamplerState*			m_pSamplerState;
		const VertexFormat*			m_pVertexFormat;
		const VertexInputBinding*	m_pVertexInputBinding;

		ConstantBuffer				m_vertexConstantBuffer;

		void						setState() const;

	};
}

#endif // TIKI_IMMEDIATERENDERER_HPP__INCLUDED
