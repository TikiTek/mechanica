#include "tiki/gameplay/level_renderer.hpp"

#include "tiki/graphics/texture.hpp"
#include "tiki/math/axis_aligned_rectangle.hpp"

#include "mechanica_base.hpp"

namespace tiki
{

	bool LevelRenderer::create( const LevelThemeData& levelTheme )
	{
		m_pLevelTheme	= &levelTheme;

		return true;
	}

	void LevelRenderer::dispose()
	{
		m_pLevelTheme	= nullptr;
	}

	void LevelRenderer::update( const GameTime& gameTime )
	{
		if( m_pLevelTheme == nullptr )
		{
			return;
		}

	}

	void LevelRenderer::render( Renderer2d& renderer )
	{
		if( m_pLevelTheme == nullptr )
		{
			return;
		}

		const Vector2 renderSize			= renderer.getVectorSize();
		const Vector2 backgroundSize		= m_pLevelTheme->background->getTextureData().getVectorSize();
		const AxisAlignedRectangle backgroundRectangle = AxisAlignedRectangle::createFill( renderSize, backgroundSize );
		renderer.queueSprite( m_pLevelTheme->background->getTextureData(), backgroundRectangle, MechanicaRenderLayer_BackgroundColor );
	}
}
