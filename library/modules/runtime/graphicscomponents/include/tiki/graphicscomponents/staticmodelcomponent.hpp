#ifndef TIKI_STATICMODELCOMPONENT_HPP
#define TIKI_STATICMODELCOMPONENT_HPP

#include "tiki/component/component.hpp"
#include "tiki/graphics/sampler.hpp"

#include "tiki/graphicsbase/color.hpp"

namespace tiki
{
	class GpuContext;
	class Model;
	class Material;
	class Transform;
	class VertexFormat;

	class StaticModelComponent : public IComponent
	{
		TIKI_DEFINE_COMPONENT( StaticModelComponent );

	public:

											~StaticModelComponent();

		void								initialize( const Transform* pTransform, const Model* pModel );
		void								dispose();

		void								setColor( const Color& color ) { m_color = color; }

		void								show() { m_visibile = true; }
		void								hide() { m_visibile = false; }

		static void							initializeSystem();
		static void							disposeSystem();

		static void							render( GpuContext* pContext );

		const Transform*					getTransform() const { return m_pTransform; }

	private:
											StaticModelComponent();

		const Transform*					m_pTransform;
		const Model*						m_pModel;

		Color								m_color;
		bool								m_visibile;

		static Sampler						s_sampler;

	};
}

#endif // TIKI_STATICMODELCOMPONENT_HPP