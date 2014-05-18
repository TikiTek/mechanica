#pragma once
#ifndef TIKI_GAMECLIENT_HPP__INCLUDED
#define TIKI_GAMECLIENT_HPP__INCLUDED

#include "tiki/components/physicsbodycomponent.hpp"
#include "tiki/components/physicscollidercomponent.hpp"
#include "tiki/components/skinnedmodelcomponent.hpp"
#include "tiki/components/staticmodelcomponent.hpp"
#include "tiki/components/transformcomponent.hpp"
#include "tiki/entitysystem/entitysystem.hpp"
#include "tiki/physics/physicsworld.hpp"

namespace tiki
{
	class Model;

	class GameClient
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( GameClient );

	public:

		bool						create();
		void						dispose();

		EntityId					createModelEntity( const Model* pModel, const Vector3& position );
		EntityId					createPhysicsBoxEntity( const Model* pModel, const Vector3& position );
		EntityId					createPlaneEntity( const Model* pModel, const Vector3& position );

		void						disposeEntity( EntityId entityId );

		void						update( float timeStep );
		void						render( GameRenderer& gameRenderer );

		const EntitySystem&			getEntitySystem() const	{ return m_entitySystem; }
		PhysicsWorld&				getPhysicsWorld()		{ return m_physicsWorld; }

		const TransformComponent&	getTransformComponent() const { return m_transformComponent; }

	private:

		enum 
		{
			MaxTypeCount	= 16u,
			ChunkCount		= 128u,
			ChunkSize		= 4096u
		};

		EntitySystem				m_entitySystem;

		PhysicsWorld				m_physicsWorld;

		PhysicsBodyComponent		m_physicsBodyComponent;
		PhysicsColliderComponent	m_physicsColliderComponent;
		SkinnedModelComponent		m_skinnedModelComponent;
		StaticModelComponent		m_staticModelComponent;
		TransformComponent			m_transformComponent;

	};
}

#endif // TIKI_GAMECLIENT_HPP__INCLUDED
