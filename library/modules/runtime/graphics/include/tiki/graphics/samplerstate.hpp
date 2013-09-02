#ifndef TIKI_SAMPLER_HPP__
#define TIKI_SAMPLER_HPP__

#include "graphicstypes.hpp"

namespace tiki
{
	class SamplerState
	{
		TIKI_NONCOPYABLE_CLASS( SamplerState );
		friend class GraphicsContext;
		friend class GraphicsSystem;

	public:

	protected:

		SamplerState();
		~SamplerState();

		void					create();
		void					dispose();

	private:

		TGSamplerState*			m_pSamplerState;

	};
}

#endif // TIKI_SAMPLER_HPP__
