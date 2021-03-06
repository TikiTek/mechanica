#pragma once

#include "tiki/components/component.hpp"

#include "tiki/base/random.hpp"
#include "tiki/container/array_view.hpp"
#include "tiki/math/vector.hpp"
#include "tiki/resource/resource_file.hpp"

namespace tiki
{
	class EntitySystem;
	class Physics2dBodyComponent;
	class Physics2dWorld;
	class PolygonComponent;
	class SpriteComponent;
	class Transform2dComponent;
	struct BreakableComponentInitData;
	struct BreakableComponentState;
	struct BreakableFragment;

	class BreakableComponent : public Component< BreakableComponentState, BreakableComponentInitData >
	{
		TIKI_NONCOPYABLE_CLASS( BreakableComponent );

	public:

		explicit			BreakableComponent();
		virtual				~BreakableComponent();

		bool				create( EntitySystem& entitySystem, Physics2dWorld& physicsWorld, const Transform2dComponent& transformComponent, const Physics2dBodyComponent& bodyComponent, const SpriteComponent& spriteComponent, const PolygonComponent& polygonComponent );
		void				dispose();

		void				update( double deltaTime );

		void				breakBody( BreakableComponentState* pState );

	protected:

		virtual bool		internalInitializeState( const ComponentEntityIterator& iterator, BreakableComponentState* pState, const BreakableComponentInitData* pInitData ) TIKI_OVERRIDE_FINAL;
		virtual void		internalDisposeState( BreakableComponentState* pState ) TIKI_OVERRIDE_FINAL;

	private:

		EntitySystem*						m_pEntitySystem;
		Physics2dWorld*						m_pPhysicsWorld;

		RandomNumberGenerator				m_random;

		const Transform2dComponent*			m_pTransformComponent;
		const Physics2dBodyComponent*		m_pBodyComponent;
		const SpriteComponent*				m_pSpriteComponent;
		const PolygonComponent*				m_pPolgonComponent;

		void								breakToStaticFragmentEntities( const ComponentEntityIterator& parentEntityComponentIterator, const BreakableComponentState* pParentState ) const;
		void								breakToCuttedFragmentEntities( const ComponentEntityIterator& parentEntityComponentIterator, const BreakableComponentState* pParentState );

		void								createChildBreakable( uint32 layerId, const ArrayView< float2 >& polygonPoints, const ComponentEntityIterator& parentEntityComponentIterator, const BreakableComponentState* pParentState ) const;
	};
}
