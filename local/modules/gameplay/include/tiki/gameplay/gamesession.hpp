#pragma once
#ifndef TIKI_GAMESESSION_HPP_INCLUDED
#define TIKI_GAMESESSION_HPP_INCLUDED

#include "tiki/base/types.hpp"
#include "tiki/gameplay/gameclient.hpp"

namespace tiki
{
	class GameClient;
	class Model;
	class ResourceManager;
	struct ViewData;
	struct InputEvent;
	struct TransformComponentState;

	class GameSession
	{
		TIKI_NONCOPYABLE_CLASS( GameSession );

	public:

									GameSession();
									~GameSession();

		bool						create( ResourceManager& resourceManager );
		void						dispose( ResourceManager& resourceManager );

		void						update( float timeDelta, float totalGameTime );
		void						render( GraphicsContext& graphicsContext );

		bool						processInputEvent( const InputEvent& inputEvent );

		GameClient&					getGameClient() { return m_gameClient; }
		const GameClient&			getGameClient() const { return m_gameClient; }

	private:

		GameClient					m_gameClient;
	};
}

#endif // TIKI_GAMESESSION_HPP_INCLUDED
