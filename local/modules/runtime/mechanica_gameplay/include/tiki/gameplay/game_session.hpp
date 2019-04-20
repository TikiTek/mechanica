#pragma once

#include "tiki/base/types.hpp"
#include "tiki/gameplay/game_client.hpp"

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

		bool						create( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager );
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
