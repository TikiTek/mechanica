#pragma once
#ifndef TIKI_GAMECLIENT_HPP__INCLUDED
#define TIKI_GAMECLIENT_HPP__INCLUDED

#include "tiki/entitysystem/entitysystem.hpp"
#include "tiki/components/typeregister.hpp"
#include "tiki/components/componentstorage.hpp"
#include "tiki/components/staticmodelcomponent.hpp"
#include "tiki/components/skinnedmodelcomponent.hpp"

namespace tiki
{
	class GameClient
	{
		TIKI_NONCOPYABLE_CLASS( GameClient );

	public:

		bool	create();
		void	dispose();

		void	update();
		void	render();

	private:

		enum 
		{
			MaxTypeCount	= 16u,
			ChunkCount		= 128u,
			ChunkSize		= 512u
		};

		EntitySystem			m_entitySystem;

		StaticModelComponent	m_staticModelComponent;
		SkinnedModelComponent	m_skinnedModelComponent;


	};
}

#endif // TIKI_GAMECLIENT_HPP__INCLUDED
