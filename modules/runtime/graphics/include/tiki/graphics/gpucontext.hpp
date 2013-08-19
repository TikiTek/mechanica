#ifndef TIKI_GRAPHICCOMMANDWRITER_HPP__
#define TIKI_GRAPHICCOMMANDWRITER_HPP__

#include "tiki/base/array.hpp"
#include "tiki/base/inline.hpp"
#include "tiki/base/types.hpp"
#include "tiki/graphics/linerenderer.hpp"
#include "tiki/graphics/deferredrenderer.hpp"
#include "tiki/graphics/spriterenderer.hpp"

namespace tiki
{
	class GraphicsSystem;
	class Material;
	class Shader;
	class Sampler;
	class VertexFormat;

	struct Color;
	struct GraphicsHandles;
	struct Vector2;
	struct Vector3;

	struct Rectangle;

	enum PrimitiveTopology;

	class GpuContext
	{
	public:

		GpuContext();
		~GpuContext();

		bool		create( GraphicsSystem& graphicsSystem );
		void		dispose();

		void		beginFrame();
		void		endFrame();

		void		clearRenderTarget( const RenderTarget& renderTarget, const Color& color = Color::black );
		void		clearBackBuffer( const Color& color = Color::black );
		void		clearDepthBuffer();

		void		enableDepth();
		void		disableDepth();

		void		enableAlpha();
		void		disableAlpha();

		void		setMaterial( const Material* pMaterial );
		void		setVertexShader( const Shader* pVertexShader );
		void		setPixelShader( const Shader* pPixelShader );

		void		setInputLayout( const VertexFormat* pVertexFormat );
		template<typename Buffer>
		void		setVertexBuffer( const Buffer& buffer, const uint slot = 0u );
		template<typename Buffer>
		void		setIndexBuffer( const Buffer& buffer );
		template<typename Buffer>
		void		setConstantBuffer( const Buffer& buffer, const uint slot = 0u );

		void		beginLightPass();
		void		endLightPass();

		void		mapRenderTargets();
		void		unmapRenderTargets();

		void		flushSpriteRenderer();

		void		setSampler( const Sampler& sampler, const uint slot = 0u );

		void		setVertexShaderTexture( const TextureData* pTextureData, const size_t slot = 0u );
		void		setPixelShaderTexture( const TextureData* pTextureData, const size_t slot = 0u );

		void		setPrimitiveTopology( PrimitiveTopology topology );

		void		clearRenderTargetStack();
		void		pushRenderTarget( const RenderTarget& renderTarget );
		void		pushBackBufferTarget();
		void		flushRenderTargetStack();

		void		drawIndexed( const uint indexcount, const uint startIndex = 0u, const uint basevertex = 0u );
		void		drawLine( const Vector3& start, const Vector3& end, const Color& color );
		void		draw( const uint count, const uint offset = 0u );

		void		drawText( const Vector2& position, const Font& font, const string& text );
		void		drawTexture( const Rectangle& rect, const TextureData& textureData, bool percentage = false );
		void		drawTexture( const Rectangle& dest, const Rectangle& src, const TextureData& tex );

		const Vector2&	getBackBufferSize() const;

	private:

		GraphicsSystem*					m_pGraphics;
		GraphicsHandles*				m_pHandles;

		Array< const TGRenderTarget* >	m_renderTargets;
		uint							m_renderTargetCount;


		LineRenderer					m_lineRender;
		SpritRenderer					m_spriteRenderer;
		DeferredRenderer				m_deferredRenderer;
	};
}

#include "gpucontext.inl"

#endif // TIKI_GRAPHICCOMMANDWRITER_HPP__


