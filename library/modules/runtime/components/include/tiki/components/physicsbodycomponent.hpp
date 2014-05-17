#pragma once
#ifndef __TIKI_PHYSICSBODYCOMPONENT_HPP_INCLUDED__
#define __TIKI_PHYSICSBODYCOMPONENT_HPP_INCLUDED__

#include "tiki/components/component.hpp"

namespace tiki
{
	class PhysicsWorld;
	class TransformComponent;
	struct PhysicsBodyComponentInitData;
	struct PhysicsBodyComponentState;

	class PhysicsBodyComponent : public Component< PhysicsBodyComponentState, PhysicsBodyComponentInitData >
	{
		TIKI_NONCOPYABLE_CLASS( PhysicsBodyComponent );

	public:

		explicit			PhysicsBodyComponent();
		virtual				~PhysicsBodyComponent();

		bool				create( PhysicsWorld& physicsWorld, const TransformComponent& transformComponent );
		void				dispose();

		void				update();

		virtual crc32		getTypeCrc() const;
		virtual uint32		getStateSize() const;
		virtual const char*	getTypeName() const;

	protected:

		virtual bool		internalInitializeState( ComponentEntityIterator& componentIterator, PhysicsBodyComponentState* pComponentState, const PhysicsBodyComponentInitData* pComponentInitData );
		virtual void		internalDisposeState( PhysicsBodyComponentState* pComponentState );

	private:

		PhysicsWorld*		m_pWorld;

		ComponentTypeId		m_transformTypeId;

	};
}

#endif // __TIKI_PHYSICSBODYCOMPONENT_HPP_INCLUDED__
