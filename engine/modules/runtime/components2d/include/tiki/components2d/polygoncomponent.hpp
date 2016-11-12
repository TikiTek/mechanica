#pragma once
#ifndef TIKI_POLYGONCOMPONENT_HPP_INCLUDED
#define TIKI_POLYGONCOMPONENT_HPP_INCLUDED

#include "tiki/components/component.hpp"

namespace tiki
{
	class Renderer2d;
	class Transform2dComponent;
	struct PolygonComponentInitData;
	struct PolygonComponentState;
	struct Vector2;

	class PolygonComponent : public Component< PolygonComponentState, PolygonComponentInitData >
	{
		TIKI_NONCOPYABLE_CLASS( PolygonComponent );

	public:

		explicit			PolygonComponent();
		virtual				~PolygonComponent();

		bool				create( const Transform2dComponent& transformComponent );
		void				dispose();

		void				render( Renderer2d& renderer ) const;

	protected:

		virtual bool		internalInitializeState( const ComponentEntityIterator& componentIterator, PolygonComponentState* pComponentState, const PolygonComponentInitData* pComponentInitData ) TIKI_OVERRIDE_FINAL;
		virtual void		internalDisposeState( PolygonComponentState* pComponentState ) TIKI_OVERRIDE_FINAL;

	private:

		const Transform2dComponent* m_pTransformComponent;
	};
}

#endif // TIKI_POLYGONCOMPONENT_HPP_INCLUDED
