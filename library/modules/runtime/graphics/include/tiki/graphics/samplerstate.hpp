#ifndef TIKI_SAMPLERSTATE_HPP__
#define TIKI_SAMPLERSTATE_HPP__

#include "tiki/graphicsbase/graphicsstateobject.hpp"

#include "graphicstypes.hpp"

namespace tiki
{
	enum AddressMode
	{
		AddressMode_Wrap,
		AddressMode_Mirror,
		AddressMode_Clamp,
		AddressMode_Border,

		AddressMode_Count
	};

	enum FilterMode
	{
		FilterMode_Neares,
		FilterMode_Linear,
		FilterMode_Anisotropic,

		FilterMode_Count
	};

	struct SamplerStateParamters
	{
		AddressMode addressU;
		AddressMode addressV;
		AddressMode addressW;
		
		FilterMode magFilter;
		FilterMode mipFilter;
		
		size_t maxAnisotropy;
		Color borderColor;

		SamplerStateParamters()
		{
			addressU = AddressMode_Wrap;
			addressV = AddressMode_Wrap;
			addressW = AddressMode_Wrap;
			
			magFilter = FilterMode_Linear;
			mipFilter = FilterMode_Neares;

			maxAnisotropy	= 1u;
			borderColor		= TIKI_COLOR_BLACK;
		}
	};

	class SamplerState : public GraphicsStateObject
	{
		TIKI_NONCOPYABLE_CLASS( SamplerState );
		friend class GraphicsContext;
		friend class GraphicsSystem;

	protected:

		SamplerState();
		~SamplerState();

		bool				create( GraphicsSystem& graphicsSystem, const SamplerStateParamters& creationParamter );
		void				dispose();

	private:

		TGSamplerState*		m_pSamplerState;

	};
}

#endif // TIKI_SAMPLERSTATE_HPP__
