#pragma once
#ifndef TIKI_GAMECLIENT_HPP__INCLUDED
#define TIKI_GAMECLIENT_HPP__INCLUDED

#include "tiki/components/skinnedmodelcomponent.hpp"
#include "tiki/components/staticmodelcomponent.hpp"
#include "tiki/components/transformcomponent.hpp"
#include "tiki/entitysystem/entitysystem.hpp"

namespace tiki
{
	class GameClient
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( GameClient );

	public:

		bool	create();
		void	dispose();

		void	update();
		void	render( GameRenderer& gameRenderer );

	private:

		enum 
		{
			MaxTypeCount	= 16u,
			ChunkCount		= 128u,
			ChunkSize		= 4096u
		};

		EntitySystem			m_entitySystem;

		SkinnedModelComponent	m_skinnedModelComponent;
		StaticModelComponent	m_staticModelComponent;
		TransformComponent		m_transformComponent;

	};
}

#endif // TIKI_GAMECLIENT_HPP__INCLUDED
