#pragma once

#include "tiki/base/random.hpp"
#include "tiki/base/types.hpp"

namespace tiki
{
	class Renderer2d;
	struct LevelThemeData;

	class LevelRenderer : NonCopyable
	{
	public:

		bool					create( const LevelThemeData& levelTheme );
		void					dispose();

		void					update( const MechanicaUpdateContext& updateContext );
		void					render( Renderer2d& renderer );

	private:

		struct BackgroundSprite : LinkedItem< BackgroundSprite >
		{
			uint8				spriteIndex;
			Vector2				position;
		};

		using BackgroundSpritePool = ChunkedPool< BackgroundSprite >;
		using BackgroundSpriteLayer = LinkedList< BackgroundSprite >;
		using BackgroundSpriteLayerArray = Array< BackgroundSpriteLayer >;

		const LevelThemeData*		m_pLevelTheme;

		RandomNumberGenerator		m_random;

		BackgroundSpritePool		m_backgroundSpritePool;
		BackgroundSpriteLayerArray	m_backgroundLayers;
	};
}
