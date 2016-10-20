#include "tiki/renderer2d/renderer2d.hpp"



namespace tiki
{
	bool Renderer2d::create( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourcePool, uint width, uint height, uint layerCount )
	{
		m_pGraphicsSystem = &graphicsSystem;

		if( !m_chunks.create( 16u ) || !m_layers.create( layerCount ) )
		{
			dispose( resourcePool );
			return false;
		}

		if( !resize( width, height ) )
		{
			return false;
		}

		PostProcessBloomParameters bloomParameters;
		bloomParameters.width		= width / 2u;
		bloomParameters.height		= height / 2u;
		bloomParameters.passCount	= 6u;
		if( !m_bloom.create( graphicsSystem, resourcePool, bloomParameters ) )
		{
			dispose( resourcePool );
			return false;
		}

		return true;
	}

	bool Renderer2d::createResources( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourcePool )
	{
		if( m_bloom.createResources( graphicsSystem, resourcePool ) )
		{
			dispose( resourcePool );
			return false;
		}

		return true;
	}

	void Renderer2d::dispose( ResourceRequestPool& resourcePool )
	{
		m_bloom.dispose( *m_pGraphicsSystem, resourcePool );

		m_layers.dispose();
		m_chunks.dispose();

		m_offscreenColorData.dispose( *m_pGraphicsSystem );

		m_pGraphicsSystem = nullptr;
	}

	bool Renderer2d::resize( uint width, uint height )
	{
		m_offscreenColorData.dispose( *m_pGraphicsSystem );

		return true;
	}

	void Renderer2d::queueSprite( TextureData* pTexture, const Rectangle& destinationRectangle, uint layer )
	{

	}

	void Renderer2d::queueSprite( TextureData* pTexture, const Rectangle& destinationRectangle, const AxisAlignedRectangle& sourceCoodinates, uint layer )
	{

	}

	void Renderer2d::queueSprite( TextureData* pTexture, const AxisAlignedRectangle& destinationRectangle, uint layer )
	{

	}

	void Renderer2d::queueSprite( TextureData* pTexture, const AxisAlignedRectangle& destinationRectangle, const AxisAlignedRectangle& sourceCoordinates, uint layer )
	{

	}

	void Renderer2d::render( GraphicsContext& graphicscontext ) const
	{

	}

	Renderer2d::RenderCommand& Renderer2d::allocateCommand( RenderLayer& layer )
	{
		RenderChunk* pChunk = nullptr;
		if( !layer.chunks.isEmpty() )
		{
			pChunk = &layer.chunks.getLast();
		}

		if( pChunk == nullptr || pChunk->commands.isFull() )
		{
			pChunk = m_chunks.push();
			layer.chunks.push( pChunk );
		}

		return pChunk->commands.push();
	}
}
