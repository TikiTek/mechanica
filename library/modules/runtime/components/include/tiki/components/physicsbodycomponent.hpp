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
	struct Quaternion;
	struct Vector3;

	class PhysicsBodyComponent : public Component< PhysicsBodyComponentState, PhysicsBodyComponentInitData >
	{
		TIKI_NONCOPYABLE_CLASS( PhysicsBodyComponent );

	public:

		explicit			PhysicsBodyComponent();
		virtual				~PhysicsBodyComponent();

		bool				create( PhysicsWorld& physicsWorld, const TransformComponent& transformComponent );
		void				dispose();

		void				update();

		void				applyForce( PhysicsBodyComponentState* pState, const Vector3& force ) const;

		void				getPosition( Vector3& targetPosition, const PhysicsBodyComponentState* pState ) const;
		void				getRotation( Quaternion& targetRotation, const PhysicsBodyComponentState* pState ) const;

		virtual crc32		getTypeCrc() const;
		virtual uint32		getStateSize() const;
		virtual const char*	getTypeName() const;

	protected:

		virtual bool		internalInitializeState( ComponentEntityIterator& componentIterator, PhysicsBodyComponentState* pComponentState, const PhysicsBodyComponentInitData* pComponentInitData );
		virtual void		internalDisposeState( PhysicsBodyComponentState* pComponentState );

	private:

		PhysicsWorld*				m_pPhysicsWorld;

		const TransformComponent*	m_pTranformComponent;
		ComponentTypeId				m_transformComponentTypeId;

	};
}

#endif // __TIKI_PHYSICSBODYCOMPONENT_HPP_INCLUDED__
