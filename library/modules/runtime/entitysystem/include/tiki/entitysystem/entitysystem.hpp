#pragma once
#ifndef __TIKI_ENTITYSYSTEM_HPP_INCLUDED__
#define __TIKI_ENTITYSYSTEM_HPP_INCLUDED__

#include "tiki/base/fixedsizedarray.hpp"
#include "tiki/base/sortedsizedmap.hpp"
#include "tiki/entitysystem/componentstorage.hpp"
#include "tiki/entitysystem/componenttyperegister.hpp"

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

		bool					getComponentTypeIdByCrc( ComponentTypeId& targetTypeId, crc32 componentTypeCrc ) const;

		EntityId				createEntityFromTemplate( uint targetPoolIndex, const EntityTemplate& entityTemplate );
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

		struct EntityPoolInfo
		{
			EntityId	firstId;
			EntityId	firstFreeId;
			uint16		poolSize;
			uint16		offset;
		};

		typedef SortedSizedMap< crc32, ComponentTypeId > ComponentTypeIdMapping;

		ComponentTypeRegister		m_typeRegister;
		ComponentStorage			m_storage;
		
		ComponentTypeIdMapping		m_typeMapping;

		Array< EntityPoolInfo >		m_pools;
		Array< EntityData >			m_entities;

		EntityData*					getEntityData( EntityId entityId );
		const EntityData*			getEntityData( EntityId entityId ) const;

	};
}

#endif // __TIKI_ENTITYSYSTEM_HPP_INCLUDED__
