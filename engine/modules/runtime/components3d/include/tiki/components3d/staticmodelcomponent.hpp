#ifndef TIKI_STATICMODELCOMPONENT_HPP
#define TIKI_STATICMODELCOMPONENT_HPP

#include "tiki/components/component.hpp"

namespace tiki
{
	class RenderScene;
	class Transform3dComponent;
	struct StaticModelComponentInitData;
	struct StaticModelComponentState;

	class StaticModelComponent : public Component< StaticModelComponentState, StaticModelComponentInitData >
	{
		TIKI_NONCOPYABLE_CLASS( StaticModelComponent );

	public:

		explicit			StaticModelComponent();
		virtual				~StaticModelComponent();

		bool				create( Transform3dComponent& transformComponent );
		void				dispose();

		void				render( RenderScene& scene ) const;

		virtual crc32		getTypeCrc() const;
		virtual uint32		getStateSize() const;
		virtual const char*	getTypeName() const;

	protected:

		virtual bool		internalInitializeState( ComponentEntityIterator& componentIterator, StaticModelComponentState* pComponentState, const StaticModelComponentInitData* pComponentInitData );
		virtual void		internalDisposeState( StaticModelComponentState* pComponentState );

	private:

		const Transform3dComponent*	m_pTransformComponent;

	};
}

#endif // TIKI_STATICMODELCOMPONENT_HPP