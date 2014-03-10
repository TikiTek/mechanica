#ifndef TIKI_STATICMODELCOMPONENT_HPP
#define TIKI_STATICMODELCOMPONENT_HPP

#include "tiki/components/component.hpp"

namespace tiki
{
	class GameRenderer;
	struct StaticModelComponentInitData;
	struct StaticModelComponentState;

	class StaticModelComponent : public Component< StaticModelComponentState, StaticModelComponentInitData >
	{
		TIKI_NONCOPYABLE_CLASS( StaticModelComponent );

	public:

		explicit			StaticModelComponent();
		virtual				~StaticModelComponent();

		void				render( GameRenderer& gameRenderer ) const;

		virtual uint		getStateSize() const;
		virtual crc32		getTypeCrc() const;
		virtual const char*	getTypeName() const;

	protected:

		virtual bool		internalInitializeState( StaticModelComponentState* pComponentState, const StaticModelComponentInitData* pComponentInitData );
		virtual void		internalDisposeState( StaticModelComponentState* pComponentState );

	};
}

#endif // TIKI_STATICMODELCOMPONENT_HPP