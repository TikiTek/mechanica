#pragma once
#ifndef TIKI_PHYSICS2DJOINTCOMPONENT_HPP_INCLUDED
#define TIKI_PHYSICS2DJOINTCOMPONENT_HPP_INCLUDED

#include "tiki/components/component.hpp"

namespace tiki
{
	class Physics2dWorld;
	struct Physics2dJointComponentInitData;
	struct Physics2dJointComponentState;

	class Physics2dJointComponent : public Component< Physics2dJointComponentState, Physics2dJointComponentInitData >
	{
		TIKI_NONCOPYABLE_CLASS( Physics2dJointComponent );

	public:

		explicit						Physics2dJointComponent();
		virtual							~Physics2dJointComponent();

		bool							create( Physics2dWorld& physicsWorld );
		void							dispose();

		virtual crc32					getTypeCrc() const;
		virtual uint32					getStateSize() const;
		virtual const char*				getTypeName() const;

	protected:

		virtual bool					internalInitializeState( ComponentEntityIterator& componentIterator, Physics2dJointComponentState* pComponentState, const Physics2dJointComponentInitData* pComponentInitData );
		virtual void					internalDisposeState( Physics2dJointComponentState* pComponentState );

	private:

		Physics2dWorld*					m_pPhysicsWorld;
	};
}

#endif // TIKI_PHYSICS2DJOINTCOMPONENT_HPP_INCLUDED
