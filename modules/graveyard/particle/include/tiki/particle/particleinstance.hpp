#pragma once
#ifndef TIKI_PARTICLEINSTANCE_HPP_INCLUDED__
#define TIKI_PARTICLEINSTANCE_HPP_INCLUDED__

#include "tiki/base/types.hpp"

namespace tiki
{
	struct ParticleEffectData;
	class ParticleStorage;
	class ParticleVertexBuffer;

	class ParticleInstance
	{
		TIKI_NONCOPYABLE_CLASS( ParticleInstance );

	public:

		bool	create( const ParticleEffectData* pData );
		void	dispose();

		void	update( ParticleStorage& particles );
		void	render( ParticleVertexBuffer& vertexBuffer );

	private:

		//struct 

	};
}

#endif // TIKI_PARTICLEINSTANCE_HPP_INCLUDED__
