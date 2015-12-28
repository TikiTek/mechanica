#pragma once
#ifndef TIKI_IMMEDIATERENDERER_HPP__INCLUDED
#define TIKI_IMMEDIATERENDERER_HPP__INCLUDED

#include "tiki/container/sizedarray.hpp"
#include "tiki/base/string.hpp"
#include "tiki/base/types.hpp"
#include "tiki/graphics/constantbuffer.hpp"
#include "tiki/graphics/vertexbuffer.hpp"
#include "tiki/graphics/color.hpp"
#include "tiki/math/intersection.hpp"

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
	struct Box;
	struct Matrix43;
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

		void				beginRendering( GraphicsContext& graphicsContext ) const;
		void				endRendering() const;

		void				beginRenderPass( const RenderTarget* pRenderTarget = nullptr, const Camera* pProjection = nullptr ) const;
		void				endRenderPass() const;

		void				setBlendState( ImmediateBlendState blendState ) const;
		void				setDepthState( ImmediateDepthState depthState ) const;

		void				drawRectangle( const Rectangle& dest, Color color = TIKI_COLOR_WHITE ) const;
		void				drawTexturedRectangle( const TextureData& texture, const Rectangle& dest, Color color = TIKI_COLOR_WHITE ) const;
		void				drawTexturedRectangle( const TextureData& texture, const Rectangle& dest, const Rectangle& src, Color color = TIKI_COLOR_WHITE ) const;
		void				drawText( const Vector2& position, const Font& font, const char* pText, Color color = TIKI_COLOR_WHITE ) const;
		void				drawLines( const Vector3* pPoints, uint capacity, Color color = TIKI_COLOR_WHITE ) const;
		void				drawRay( const Ray3& ray, float length = 100.0f, Color color = TIKI_COLOR_WHITE ) const;
		void				drawBox( const Box& box, Color color = TIKI_COLOR_WHITE ) const;
		void				drawGrid( int gridSpacing = 5, int gridSize = 10, Color color = TIKI_COLOR_WHITE ) const;
		void				drawAxes( float lineLength, float lineOffset, const Matrix43& worldMatrix) const;
		void				drawCircle( const Vector3& center, float radius, const Vector3& axe1, const Vector3& axe2, Color color = TIKI_COLOR_WHITE ) const;
		void				drawSphere( const Vector3& center, float radius, Color color = TIKI_COLOR_WHITE ) const;

	private:

		struct ImmediateVertex
		{
			float3	position;
			uint16	u;
			uint16	v;
			Color	color;
		};

		mutable GraphicsContext*			m_pContext;

		const ShaderSet*					m_pShaderSet;
		
		const BlendState*					m_pBlendState[ ImmediateBlendState_Count ];
		const DepthStencilState*			m_pDepthStencilState[ ImmediateDepthState_Count ];
		const RasterizerState*				m_pRasterizerState;
		const SamplerState*					m_pSamplerState;
		const VertexFormat*					m_pVertexFormat;
		const VertexInputBinding*			m_pVertexInputBinding;

		ConstantBuffer						m_vertexConstantBuffer;

		void								setState() const;

	};
}

#endif // TIKI_IMMEDIATERENDERER_HPP__INCLUDED
