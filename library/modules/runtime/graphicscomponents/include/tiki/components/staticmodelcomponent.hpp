#ifndef TIKI_STATICMODELCOMPONENT_HPP
#define TIKI_STATICMODELCOMPONENT_HPP

#include "tiki/components/componentbase.hpp"

//#include "tiki/graphics/sampler.hpp"
//#include "tiki/graphicsbase/color.hpp"

namespace tiki
{
	struct StaticModelComponentState;
	struct StaticModelComponentInitData;

	class StaticModelComponent : public Component< StaticModelComponentState, StaticModelComponentInitData >
	{
		TIKI_NONCOPYABLE_CLASS( StaticModelComponent );

	//public:

	//										~StaticModelComponent();

	//	void								initialize( const Transform* pTransform, const Model* pModel );
	//	void								dispose();

	//	void								setColor( const Color& color ) { m_color = color; }

	//	void								show() { m_visibile = true; }
	//	void								hide() { m_visibile = false; }

	//	static void							initializeSystem();
	//	static void							disposeSystem();

	//	static void							render( GpuContext* pContext );

	//	const Transform*					getTransform() const { return m_pTransform; }

	//private:
	//										StaticModelComponent();

	//	const Transform*					m_pTransform;
	//	const Model*						m_pModel;

	//	Color								m_color;
	//	bool								m_visibile;

	//	static Sampler						s_sampler;

	};
}

#endif // TIKI_STATICMODELCOMPONENT_HPP