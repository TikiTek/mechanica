#pragma once

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

	protected:

		virtual bool					internalInitializeState( const ComponentEntityIterator& componentIterator, Physics2dJointComponentState* pComponentState, const Physics2dJointComponentInitData* pComponentInitData ) TIKI_OVERRIDE_FINAL;
		virtual void					internalDisposeState( Physics2dJointComponentState* pComponentState ) TIKI_OVERRIDE_FINAL;

	private:

		Physics2dWorld*					m_pPhysicsWorld;
	};
}
