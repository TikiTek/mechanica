#ifndef TIKI_GRAPHICCOMMANDWRITER_HPP__
#define TIKI_GRAPHICCOMMANDWRITER_HPP__

#include "tiki/base/array.hpp"
#include "tiki/base/inline.hpp"
#include "tiki/base/types.hpp"
#include "tiki/graphicsbase/color.hpp"
#include "tiki/graphicsbase/graphissystemlimits.hpp"

namespace tiki
{
	class ConstantBuffer;
	class GraphicsSystem;
	class IndexBuffer;
	class Material;
	class RenderTarget;
	class SamplerState;
	class Shader;
	class TextureData;
	class VertexBuffer;
	class VertexFormat;
	enum PrimitiveTopology;
	struct GraphicsHandles;
	struct Rectangle;
	struct Vector2;
	struct Vector3;

	enum ClearMask
	{
		ClearMask_Color			= 1u << 0u,
		ClearMask_Depth			= 1u << 1u,
		ClearMask_Stencil		= 1u << 2u,

		ClearMask_All			= ClearMask_Color | ClearMask_Depth | ClearMask_Stencil,
		ClearMask_DepthStencil	= ClearMask_Depth | ClearMask_Stencil
	};

	class GraphicsContext
	{
		friend class GraphicsSystem;

	public:

		bool				create( GraphicsSystem& graphicsSystem );
		void				dispose();

		void				clear( const RenderTarget& renderTarget, const Color& color = Color::black, float depthValue = 1.0f, uint8 stencilValue = 0u, ClearMask clearMask = ClearMask_All );

		void				beginRenderPass( const RenderTarget& renderTarget );
		void				endRenderPass();

		void				setVertexShader( const Shader* pVertexShader );
		void				setPixelShader( const Shader* pPixelShader );

		void				setInputLayout( const VertexFormat& vertexFormat );

		void				setIndexBuffer( const IndexBuffer& buffer );
		void				setVertexBuffer( size_t slot, const VertexBuffer& buffer );
		void				setConstantBuffer( size_t slot, const ConstantBuffer& buffer );

		void				setSamplerState( size_t slot, const SamplerState& sampler );

		void				setVertexShaderTexture( size_t slot, const TextureData* pTextureData );
		void				setPixelShaderTexture( size_t slot, const TextureData* pTextureData );

		void				setPrimitiveTopology( PrimitiveTopology topology );

		void				drawGeometry( const uint indexcount, const uint startIndex = 0u, const uint basevertex = 0u );
		void				drawIndexed( const Vector3& start, const Vector3& end, const Color& color );

		const RenderTarget*	getBackBuffer() const;

	protected:

							GraphicsContext();
							~GraphicsContext();

	private:

		GraphicsSystem*		m_pGraphics;
		GraphicsHandles*	m_pHandles;

		const RenderTarget*	m_pRenderTarget;

		const SamplerState*	m_pVertexSamplerStates[ GraphicsSystemLimits_VertexShaderTextureSlots ];
		const SamplerState*	m_pVertexTextures[ GraphicsSystemLimits_VertexShaderTextureSlots ];

		const SamplerState*	m_pPixelSamplerStates[ GraphicsSystemLimits_PixelShaderTextureSlots ];
		const SamplerState*	m_pPixelTextures[ GraphicsSystemLimits_PixelShaderTextureSlots ];

	};
}

#endif // TIKI_GRAPHICCOMMANDWRITER_HPP__


