#pragma once
#ifndef TIKI_PHYSICS3DCHARACTERCONTROLLERCOMPONENT_HPP_INCLUDED
#define TIKI_PHYSICS3DCHARACTERCONTROLLERCOMPONENT_HPP_INCLUDED

#include "tiki/components/component.hpp"

namespace tiki
{
	class Physics3dCollisionObject;
	class Physics3dWorld;
	class TransformComponent;
	struct Physics3dCharacterControllerComponentInitData;
	struct Physics3dCharacterControllerComponentState;
	struct Quaternion;
	struct Vector3;

	class Physics3dCharacterControllerComponent : public Component< Physics3dCharacterControllerComponentState, Physics3dCharacterControllerComponentInitData >
	{
		TIKI_NONCOPYABLE_CLASS( Physics3dCharacterControllerComponent );

	public:

		explicit						Physics3dCharacterControllerComponent();
		virtual							~Physics3dCharacterControllerComponent();

		bool							create( Physics3dWorld& physicsWorld, const TransformComponent& transformComponent );
		void							dispose();

		void							update();

		void							move( Physics3dCharacterControllerComponentState* pState, const Vector3& direction ) const;
		void							jump( Physics3dCharacterControllerComponentState* pState ) const;

		void							getPosition( Vector3& targetPosition, const Physics3dCharacterControllerComponentState* pState ) const;
		void							getRotation( Quaternion& targetRotation, const Physics3dCharacterControllerComponentState* pState ) const;

		const Physics3dCollisionObject&	getPhysicsObject( const Physics3dCharacterControllerComponentState* pState ) const;

		void							setRotation( Physics3dCharacterControllerComponentState* pState, const Quaternion& rotation ) const;

		virtual crc32					getTypeCrc() const;
		virtual uint32					getStateSize() const;
		virtual const char*				getTypeName() const;

	protected:

		virtual bool					internalInitializeState( ComponentEntityIterator& componentIterator, Physics3dCharacterControllerComponentState* pComponentState, const Physics3dCharacterControllerComponentInitData* pComponentInitData );
		virtual void					internalDisposeState( Physics3dCharacterControllerComponentState* pComponentState );

	private:

		Physics3dWorld*					m_pPhysicsWorld;

		const TransformComponent*		m_pTranformComponent;

	};
}

#endif // TIKI_PHYSICS3DCHARACTERCONTROLLERCOMPONENT_HPP_INCLUDED
