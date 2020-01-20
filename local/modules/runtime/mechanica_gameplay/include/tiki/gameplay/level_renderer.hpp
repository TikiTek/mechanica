#pragma once

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

		void					update( const GameTime& gameTime );
		void					render( Renderer2d& renderer );

	private:

		struct BackgroundSprite
		{
			uintreg				layerIndex;
			s
		};

		const LevelThemeData*	m_pLevelTheme;
	};
}
