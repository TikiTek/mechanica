#pragma once
#ifndef __TIKI_ENTITYSYSTEM_HPP_INCLUDED__
#define __TIKI_ENTITYSYSTEM_HPP_INCLUDED__

#include "tiki/base/fixedsizedarray.hpp"
#include "tiki/components/componentstorage.hpp"
#include "tiki/components/typeregister.hpp"

namespace tiki
{
	struct EntityTemplate;

	enum
	{
		EntitySystemLimits_MaxEntityPoolCount	= 8u
	};

	struct EntityPool
	{
		EntityId	firstId;
		uint16		poolSize;
	};

	struct EntitySystemParameters
	{
		typedef FixedSizedArray< EntityPool, EntitySystemLimits_MaxEntityPoolCount > EntityPoolArray;

		uint				typeRegisterMaxCount;
		uint				storageChunkSize;
		uint				storageChunkCount;

		EntityPoolArray		entityPools;
	};

	class EntitySystem
	{
		TIKI_NONCOPYABLE_CLASS( EntitySystem );

	public:

		EntitySystem();
		~EntitySystem();

		bool					create( const EntitySystemParameters& parameters );
		void					dispose();

		bool					registerComponentType( ComponentBase* pComponent );
		void					unregisterComponentType( ComponentBase* pComponent );

		EntityId				createEntityFromTemplate( const EntityTemplate& entityTemplate );
		void					destroyEntity( EntityId entityId );

		ComponentState*			getFirstComponentOfEntity( EntityId entityId );
		const ComponentState*	getFirstComponentOfEntity( EntityId entityId ) const;
		ComponentState*			getFirstComponentOfEntityAndType( EntityId entityId, ComponentTypeId typeId );
		const ComponentState*	getFirstComponentOfEntityAndType( EntityId entityId, ComponentTypeId typeId ) const;
		
	private:

		struct EntityData
		{
			EntityId			id;
			ComponentState*		pFirstComponent;
		};

		ComponentTypeRegister	m_typeRegister;
		ComponentStorage		m_storage;
		
		Array< EntityPool >		m_pools;
		Array< EntityData >		m_entities;

		EntityData*				getEntityData( EntityId entityId );

	};
}

#endif // __TIKI_ENTITYSYSTEM_HPP_INCLUDED__
