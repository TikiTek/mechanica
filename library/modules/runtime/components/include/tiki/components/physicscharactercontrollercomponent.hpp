#pragma once
#ifndef __TIKI_PHYSICSCHARACTERCONTROLLERCOMPONENT_HPP_INCLUDED__
#define __TIKI_PHYSICSCHARACTERCONTROLLERCOMPONENT_HPP_INCLUDED__

#include "tiki/components/component.hpp"

namespace tiki
{
	class PhysicsWorld;
	class TransformComponent;
	struct PhysicsCharacterControllerComponentInitData;
	struct PhysicsCharacterControllerComponentState;
	struct Quaternion;
	struct Vector3;

	class PhysicsCharacterControllerComponent : public Component< PhysicsCharacterControllerComponentState, PhysicsCharacterControllerComponentInitData >
	{
		TIKI_NONCOPYABLE_CLASS( PhysicsCharacterControllerComponent );

	public:

		explicit			PhysicsCharacterControllerComponent();
		virtual				~PhysicsCharacterControllerComponent();

		bool				create( PhysicsWorld& physicsWorld, const TransformComponent& transformComponent );
		void				dispose();

		void				update();

		void				move( PhysicsCharacterControllerComponentState* pState, const Vector3& direction ) const;
		void				jump( PhysicsCharacterControllerComponentState* pState ) const;

		void				getPosition( Vector3& targetPosition, const PhysicsCharacterControllerComponentState* pState ) const;
		void				getRotation( Quaternion& targetRotation, const PhysicsCharacterControllerComponentState* pState ) const;

		void				setRotation( PhysicsCharacterControllerComponentState* pState, const Quaternion& rotation ) const;

		virtual crc32		getTypeCrc() const;
		virtual uint32		getStateSize() const;
		virtual const char*	getTypeName() const;

	protected:

		virtual bool		internalInitializeState( ComponentEntityIterator& componentIterator, PhysicsCharacterControllerComponentState* pComponentState, const PhysicsCharacterControllerComponentInitData* pComponentInitData );
		virtual void		internalDisposeState( PhysicsCharacterControllerComponentState* pComponentState );

	private:

		PhysicsWorld*				m_pPhysicsWorld;

		const TransformComponent*	m_pTranformComponent;
		ComponentTypeId				m_transformComponentTypeId;

	};
}

#endif // __TIKI_PHYSICSCHARACTERCONTROLLERCOMPONENT_HPP_INCLUDED__
