#ifndef TIKI_SAMPLER_HPP__
#define TIKI_SAMPLER_HPP__

#include "graphicstypes.hpp"

namespace tiki
{
	class Sampler
	{
	public:
								Sampler();
								~Sampler();

		void					create();
		void					dispose();

		TGSamplerState*const*	getState() const { return (TGSamplerState*const*)&m_pSampler; }

	private:

		TGSamplerState*			m_pSampler;
	};
}

#endif // TIKI_SAMPLER_HPP__
