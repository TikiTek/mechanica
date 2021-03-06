#pragma once

#include "tiki/container/fixed_sized_array.hpp"
#include "tiki/container/sized_array.hpp"
#include "tiki/container/sorted_sized_map.hpp"
#include "tiki/entity_system/component_storage.hpp"
#include "tiki/entity_system/component_type_register.hpp"

namespace tiki
{
	struct EntityTemplateData;

	enum
	{
		EntitySystemLimits_MaxEntityPoolCount			= 8u,
		EntitySystemLimits_MaxEntitiesDeletePerFrame	= 64u
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

		void					update();

		bool					registerComponentType( ComponentBase* pComponent );
		void					unregisterComponentType( ComponentBase* pComponent );

		bool					getComponentTypeIdByCrc( ComponentTypeId& targetTypeId, crc32 componentTypeCrc ) const;

		EntityId				createEntityFromTemplate( uint targetPoolIndex, const EntityTemplateData& entityTemplate );
		void					disposeEntity( EntityId entityId );

		uint16					getPoolIndexForId( EntityId entityId ) const;

		ComponentState*			getFirstComponentOfEntity( EntityId entityId );
		const ComponentState*	getFirstComponentOfEntity( EntityId entityId ) const;
		ComponentState*			getFirstComponentOfEntityAndType( EntityId entityId, ComponentTypeId typeId );
		const ComponentState*	getFirstComponentOfEntityAndType( EntityId entityId, ComponentTypeId typeId ) const;

	private:

		struct EntityData
		{
			EntityId			id;
			ComponentState*		pFirstComponent;
			ComponentState*		pLastComponent;
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

		SizedArray< EntityId >		m_entitiesToDeletion;

		EntityPoolInfo*				findEntityPool( EntityId entityId );
		const EntityPoolInfo*		findEntityPool( EntityId entityId ) const;

		EntityData*					findEntityData( EntityId entityId );
		const EntityData*			findEntityData( EntityId entityId ) const;

		EntityData*					getEntityDataInPool( const EntityPoolInfo* pEntityPool, EntityId entityId );
		const EntityData*			getEntityDataInPool( const EntityPoolInfo* pEntityPool, EntityId entityId ) const;

		void						disposeEntityFinally( EntityId entityId );
	};
}
