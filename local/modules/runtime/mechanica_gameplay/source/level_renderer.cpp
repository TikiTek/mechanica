#include "tiki/gameplay/level_renderer.hpp"

#include "tiki/graphics/texture.hpp"
#include "tiki/math/axis_aligned_rectangle.hpp"
#include "tiki/mechanica_types/mechanica_units.hpp"

#include "mechanica_base.hpp"

namespace tiki
{

	bool LevelRenderer::create( const LevelThemeData& levelTheme )
	{
		m_pLevelTheme	= &levelTheme;

		m_random.createFromTime();

		m_backgroundSpritePool.create( 64u );
		m_backgroundLayers.create( levelTheme.backgroundLayers.getCount() );

		return true;
	}

	void LevelRenderer::dispose()
	{
		for( BackgroundSpriteLayer& layer : m_backgroundLayers )
		{
			while( !layer.isEmpty() )
			{
				BackgroundSprite* pSprite = layer.popFirst();
				m_backgroundSpritePool.removeUnsortedByValue( pSprite );
			}
		}

		m_backgroundLayers.dispose();
		m_backgroundSpritePool.dispose();

		m_pLevelTheme = nullptr;
	}

	void LevelRenderer::update( const MechanicaUpdateContext& updateContext )
	{
		if( m_pLevelTheme == nullptr )
		{
			return;
		}

		const Vector2 halfRenderSize = Vector2::scale( updateContext.renderUnitSize, 0.5f);
		for( uint i = 0u; i < m_backgroundLayers.getCount(); ++i )
		{
			const LevelThemeBackgroundLayer& themeLayer = m_pLevelTheme->backgroundLayers[ i ];
			BackgroundSpriteLayer& layer = m_backgroundLayers[ i ];

			float maxX = -halfRenderSize.x - themeLayer.maxDistanceX;
			float endX = -halfRenderSize.x - themeLayer.maxDistanceX;
			BackgroundSprite* pNextSprite = nullptr;
			for( BackgroundSprite* pSprite = layer.getFirst(); pSprite != nullptr; pSprite = pNextSprite )
			{
				pNextSprite = layer.getNext( pSprite );

				const LevelThemeBackgroundSprite& themeSprite = themeLayer.sprites[ pSprite->spriteIndex ];
				const Vector2 halfSpriteSize = mech::toUnitSize( Vector2::mul( themeSprite.texture->getTextureData().getVectorSize(), Vector2::create( themeSprite.scale * 0.5f ) ) );

				if( themeLayer.bottomPositionY )
				{
					pSprite->position.y = halfRenderSize.y;
				}

				pSprite->position.x -= float( themeLayer.speed * updateContext.gameTime.elapsedTime );

				const float spriteEndX = pSprite->position.x + halfSpriteSize.x;
				if( spriteEndX < -halfRenderSize.x )
				{
					layer.removeSortedByValue( pSprite );
					m_backgroundSpritePool.removeUnsortedByValue( pSprite );
				}

				maxX = max( maxX, pSprite->position.x );
				endX = max( endX, spriteEndX );
			}

			while( endX < updateContext.renderUnitSize.x + themeLayer.maxDistanceX )
			{
				const float distance		= m_random.nextUniformFloat32( themeLayer.minDistanceX, themeLayer.maxDistanceX );
				const uint64 spriteIndex	= m_random.nextUniformUInt64( 0u, themeLayer.sprites.getCount() );

				float positionY;
				if( themeLayer.bottomPositionY )
				{
					positionY = halfRenderSize.y;
				}
				else
				{
					positionY =  m_random.nextUniformFloat32( themeLayer.minPositionY, themeLayer.maxPositionY );
				}

				BackgroundSprite* pSprite = m_backgroundSpritePool.push();
				pSprite->spriteIndex	= uint8( spriteIndex );
				pSprite->position		= Vector2::create( maxX + distance, positionY );

				layer.push( pSprite );

				const LevelThemeBackgroundSprite& themeSprite	= themeLayer.sprites[ pSprite->spriteIndex ];
				const Vector2 halfSpriteSize					= mech::toUnitSize( Vector2::mul( themeSprite.texture->getTextureData().getVectorSize(), Vector2::create( themeSprite.scale * 0.5f ) ) );
				const float spriteEndX							= pSprite->position.x + halfSpriteSize.x;
				maxX = max( maxX, pSprite->position.x );
				endX = max( endX, spriteEndX );
			}
		}
	}

	void LevelRenderer::render( Renderer2d& renderer )
	{
		if( m_pLevelTheme == nullptr )
		{
			return;
		}

		{
			const Vector2 renderSize						= mech::toUnitSize( renderer.getVectorSize() );
			const Vector2 backgroundSize					= mech::toUnitSize( m_pLevelTheme->background->getTextureData().getVectorSize() );
			const AxisAlignedRectangle backgroundRectangle	= AxisAlignedRectangle::createFill( renderSize, backgroundSize );
			renderer.queueSprite( m_pLevelTheme->background->getTextureData(), backgroundRectangle, MechanicaRenderLayer_BackgroundColor );
		}

		for( uint i = 0u; i < m_backgroundLayers.getCount(); ++i )
		{
			const LevelThemeBackgroundLayer& themeLayer = m_pLevelTheme->backgroundLayers[ i ];
			BackgroundSpriteLayer& layer = m_backgroundLayers[ i ];

			for( const BackgroundSprite& sprite : layer )
			{
				const LevelThemeBackgroundSprite& themeSprite = themeLayer.sprites[ sprite.spriteIndex ];

				const float unitScale						= mech::toUnitSize( themeSprite.scale );
				const Vector2 spriteSize					= Vector2::mul( themeSprite.texture->getTextureData().getVectorSize(), Vector2::create( unitScale ) );
				const AxisAlignedRectangle spriteRectangle	= AxisAlignedRectangle::createCentered( sprite.position, spriteSize );

				renderer.queueSprite( themeSprite.texture->getTextureData(), spriteRectangle, MechanicaRenderLayer_BackgroundIslands );
			}
		}
	}
}
