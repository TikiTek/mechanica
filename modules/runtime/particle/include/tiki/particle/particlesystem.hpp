#pragma once
#ifndef TIKI_PARTICLESYSTEM_HPP_INCLUDED__
#define TIKI_PARTICLESYSTEM_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/base/linkedlist.hpp"

namespace tiki
{
	class ParticleInstance;

	class ParticleSystem
	{
		TIKI_NONCOPYABLE_CLASS( ParticleSystem );

	public:

		bool	create();
		void	dispose();

		void	update( float deltaTime );

	private:

		LinkedList<ParticleInstance>	m_instances;

	};
}

#endif // TIKI_PARTICLESYSTEM_HPP_INCLUDED__
