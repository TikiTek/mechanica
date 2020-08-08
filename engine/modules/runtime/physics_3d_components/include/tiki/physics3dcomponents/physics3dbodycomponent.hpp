#pragma once
#ifndef TIKI_PHYSICS3DBODYCOMPONENT_HPP_INCLUDED
#define TIKI_PHYSICS3DBODYCOMPONENT_HPP_INCLUDED

#include "tiki/components/component.hpp"

namespace tiki
{
	class Physics3dCollisionObject;
	class Physics3dWorld;
	class Transform3dComponent;
	struct Physics3dBodyComponentInitData;
	struct Physics3dBodyComponentState;
	struct Quaternion;
	struct Vector3;

	class Physics3dBodyComponent : public Component< Physics3dBodyComponentState, Physics3dBodyComponentInitData >
	{
		TIKI_NONCOPYABLE_CLASS( Physics3dBodyComponent );

	public:

		explicit						Physics3dBodyComponent();
		virtual							~Physics3dBodyComponent();

		bool							create( Physics3dWorld& physicsWorld, const Transform3dComponent& transformComponent );
		void							dispose();

		void							update();

		void							applyForce( Physics3dBodyComponentState* pState, const Vector3& force ) const;

		void							getPosition( Vector3& targetPosition, const Physics3dBodyComponentState* pState ) const;
		void							getRotation( Quaternion& targetRotation, const Physics3dBodyComponentState* pState ) const;

		const Physics3dCollisionObject&	getPhysicsObject( const Physics3dBodyComponentState* pState ) const;

		virtual crc32					getTypeCrc() const;
		virtual uint32					getStateSize() const;
		virtual const char*				getTypeName() const;

	protected:

		virtual bool					internalInitializeState( ComponentEntityIterator& componentIterator, Physics3dBodyComponentState* pComponentState, const Physics3dBodyComponentInitData* pComponentInitData );
		virtual void					internalDisposeState( Physics3dBodyComponentState* pComponentState );

	private:

		Physics3dWorld*				m_pPhysicsWorld;

		const Transform3dComponent*	m_pTranformComponent;

	};
}

#endif // TIKI_PHYSICS3DBODYCOMPONENT_HPP_INCLUDED
