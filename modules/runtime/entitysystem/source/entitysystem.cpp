#include "tiki/entitysystem/entitysystem.hpp"

#include "tiki/components/component.hpp"

#include "entities.hpp"

namespace tiki
{
	EntitySystem::EntitySystem()
	{
	}

	EntitySystem::~EntitySystem()
	{
	}

	bool EntitySystem::create( const EntitySystemParameters& parameters )
	{
		if ( parameters.entityPools.getCount() == 0u )
		{
			TIKI_TRACE_ERROR( "[entitysystem] No pols defined. EntitySystem could not created.\n" );
			return false;
		}

		if ( !m_typeRegister.create( parameters.typeRegisterMaxCount ) )
		{
			dispose();
			return false;
		}

		if ( !m_typeMapping.create( parameters.typeRegisterMaxCount ) )
		{
			dispose();
			return false;
		}

		if ( !m_storage.create( parameters.storageChunkSize, parameters.storageChunkCount, m_typeRegister ) )
		{
			dispose();
			return false;
		}

		uint16 entityCapacity = 0u;
		if ( m_pools.create( parameters.entityPools.getCount() + 1u ) )
		{
			EntityPoolInfo& firstPool = m_pools[ 0u ];
			firstPool.firstId		= InvalidEntityId;
			firstPool.firstFreeId	= InvalidEntityId;
			firstPool.poolSize		= 1u;
			firstPool.offset		= 0u;

			// TODO: more checks of integrity
			for (uint i = 0u; i < parameters.entityPools.getCount(); ++i)
			{
				const EntityPool& pool = parameters.entityPools[ i ];

				EntityPoolInfo& targetPool = m_pools[ i + 1u ];
				targetPool.firstId		= pool.firstId;
				targetPool.firstFreeId	= pool.firstId;
				targetPool.poolSize		= pool.poolSize;
				targetPool.offset		= entityCapacity;

				entityCapacity += pool.poolSize;
			}
		}
		else
		{
			dispose();
			return false;
		}

		if ( m_entities.create( entityCapacity ) )
		{
			for (uint i = 0u; i < m_entities.getCount(); ++i)
			{
				EntityData& entityData = m_entities[ i ];

				entityData.id				= InvalidEntityId;
				entityData.pFirstComponent	= nullptr;
			}
		}
		else
		{
			dispose();
			return false;
		}

		if ( !m_entitiesToDeletion.create( EntitySystemLimits_MaxEntitiesDeletePerFrame ) )
		{
			dispose();
			return false;
		}

		return true;
	}

	void EntitySystem::dispose()
	{
		TIKI_ASSERT( m_entitiesToDeletion.isEmpty() );
		m_entitiesToDeletion.dispose();

		m_entities.dispose();
		m_pools.dispose();

		m_storage.dispose();
		m_typeMapping.dispose();
		m_typeRegister.dispose();
	}

	void EntitySystem::update()
	{
		for (uint i = 0u; i < m_entitiesToDeletion.getCount(); ++i)
		{
			disposeEntityFinally( m_entitiesToDeletion[ i ] );
		}
		m_entitiesToDeletion.clear();
	}

	bool EntitySystem::registerComponentType( ComponentBase* pComponent )
	{
		TIKI_ASSERT( pComponent != nullptr );

		const ComponentTypeId typeId = m_typeRegister.registerType( pComponent );
		if ( typeId != InvalidComponentTypeId )
		{
			m_typeMapping.set( pComponent->getTypeCrc(), typeId );
			return true;
		}

		return false;
	}

	void EntitySystem::unregisterComponentType( ComponentBase* pComponent )
	{
		TIKI_ASSERT( pComponent != nullptr );

		ComponentTypeId typeId;
		const crc32 componentTypeCrc = pComponent->getTypeCrc();
		if ( m_typeMapping.findValue( &typeId, componentTypeCrc ) )
		{
			m_typeRegister.unregisterType( typeId );
			m_typeMapping.remove( componentTypeCrc );
		}
	}

	bool EntitySystem::getComponentTypeIdByCrc( ComponentTypeId& targetTypeId, crc32 componentTypeCrc ) const
	{
		return m_typeMapping.findValue( &targetTypeId, componentTypeCrc );
	}

	EntityId EntitySystem::createEntityFromTemplate( uint targetPoolIndex, const EntityTemplateData& entityTemplate )
	{
		TIKI_ASSERT( targetPoolIndex < m_pools.getCount() );

		EntityPoolInfo& pool = m_pools[ targetPoolIndex + 1u ];
		if ( pool.firstFreeId == InvalidEntityId )
		{
			return InvalidEntityId;
		}

		const EntityId entityId		= pool.firstFreeId;
		const uint16 entityIndex	= pool.offset + ( entityId - pool.firstId );

		// find next free id
		pool.firstFreeId = InvalidEntityId;
		const uint16 maxEntityIndexInPool = pool.offset + pool.poolSize;
		for (uint16 i = entityIndex + 1u; i < maxEntityIndexInPool; ++i)
		{
			if ( m_entities[ i ].id == InvalidEntityId )
			{
				pool.firstFreeId = pool.firstId + ( i - pool.offset );
				break;
			}
		}

		// create components
		ComponentState* pLastState = nullptr;
		ComponentState* pFirstState = nullptr;
		for (uint i = 0u; i < entityTemplate.components.getCount(); ++i)
		{
			const EntityComponent& entityComponent = entityTemplate.components[ i ];

			ComponentTypeId typeId;
			if ( !m_typeMapping.findValue( &typeId, entityComponent.componentType ) )
			{
				TIKI_TRACE_ERROR( "[entitysystem] Cound find component type with CRC: 0x%08x\n", entityComponent.componentType );
				continue;
			}

			ComponentState* pComponentState = m_storage.allocateState( typeId );
			if ( pComponentState == nullptr )
			{
				TIKI_TRACE_ERROR( "[entitysystem] Cound allocate component state for component with CRC: 0x%08x\n", entityComponent.componentType );
				continue;
			}
			pComponentState->entityId					= entityId;
			pComponentState->pPrevComponentOfSameEntity	= pLastState;

			// initialize state
			ComponentBase* pComponent = m_typeRegister.getTypeComponent( typeId );
			ComponentEntityIterator iterator = ComponentEntityIterator( pFirstState );
			if ( !pComponent->initializeState( iterator, pComponentState, entityComponent.initData.getData() ) )
			{
				TIKI_TRACE_ERROR( "[entitysystem] Cound initialize component state for component with CRC: 0x%08x\n", entityComponent.componentType );
				m_storage.freeState( pComponentState );
				continue;
			}
						
			if ( pLastState != nullptr )
			{
				pLastState->pNextComponentOfSameEntity = pComponentState;
			}

			if ( pFirstState == nullptr )
			{
				pFirstState = pComponentState;
			}

			pLastState = pComponentState;
		}

		EntityData& entityData = m_entities[ entityIndex ];
		entityData.id				= entityId;
		entityData.pFirstComponent	= pFirstState;
		entityData.pLastComponent	= pLastState;

		return entityId;
	}

	void EntitySystem::disposeEntity( EntityId entityId )
	{
		if( entityId == InvalidEntityId )
		{
			return;
		}

		if ( !m_entitiesToDeletion.isFull() )
		{
			m_entitiesToDeletion.push( entityId );
		}
	}

	void EntitySystem::disposeEntityFinally( EntityId entityId )
	{
		EntityPoolInfo* pEntityPool = findEntityPool( entityId );
		if ( pEntityPool == nullptr )
		{
			return;
		}

		if ( pEntityPool->firstFreeId == InvalidEntityId )
		{
			pEntityPool->firstFreeId = entityId;
		}

		EntityData* pEntityData = getEntityDataInPool( pEntityPool, entityId );
		if ( pEntityData == nullptr || pEntityData->id == InvalidEntityId )
		{
			return;
		}
		TIKI_ASSERT( pEntityData->id == entityId );


		ComponentState* pComponentState = pEntityData->pLastComponent;
		while ( pComponentState != nullptr )
		{
			ComponentState* pPrevState = pComponentState->pPrevComponentOfSameEntity;

			ComponentBase* pComponent = m_typeRegister.getTypeComponent( pComponentState->typeId );
			pComponent->disposeState( pComponentState );

			m_storage.freeState( pComponentState );

#if TIKI_ENABLED( TIKI_BUILD_DEBUG )
			pComponent->checkIntegrity();
#endif

			pComponentState = pPrevState;
		}

		pEntityData->id					= InvalidEntityId;
		pEntityData->pFirstComponent	= nullptr;
	}

	ComponentState* EntitySystem::getFirstComponentOfEntity( EntityId entityId )
	{
		EntityData* pEntityData = findEntityData( entityId );
		if ( pEntityData == nullptr )
		{
			return nullptr;
		}

		return pEntityData->pFirstComponent;
	}

	const ComponentState* EntitySystem::getFirstComponentOfEntity( EntityId entityId ) const
	{
		const EntityData* pEntityData = findEntityData( entityId );
		if ( pEntityData == nullptr )
		{
			return nullptr;
		}

		return pEntityData->pFirstComponent;
	}

	ComponentState* EntitySystem::getFirstComponentOfEntityAndType( EntityId entityId, ComponentTypeId typeId )
	{
		ComponentState* pComponentState = getFirstComponentOfEntity( entityId );
		while ( pComponentState != nullptr )
		{
			if ( pComponentState->typeId == typeId )
			{
				return pComponentState;
			}

			pComponentState = pComponentState->pNextComponentOfSameEntity;
		}

		return nullptr;
	}

	const ComponentState* EntitySystem::getFirstComponentOfEntityAndType( EntityId entityId, ComponentTypeId typeId ) const
	{
		const ComponentState* pComponentState = getFirstComponentOfEntity( entityId );
		while ( pComponentState != nullptr )
		{
			if ( pComponentState->typeId == typeId )
			{
				return pComponentState;
			}

			pComponentState = pComponentState->pNextComponentOfSameEntity;
		}

		return nullptr;
	}

	EntitySystem::EntityPoolInfo* EntitySystem::findEntityPool( EntityId entityId )
	{
		if ( entityId == InvalidEntityId )
		{
			return nullptr;
		}

		EntityPoolInfo* pEntityPool = nullptr;
		for (uint i = 0u; i < m_pools.getCount(); ++i)
		{
			EntityPoolInfo& pool = m_pools[ i ];
			if ( entityId >= pool.firstId && entityId < pool.firstId + pool.poolSize )
			{
				pEntityPool = &pool;
				break;
			}
		}

		return pEntityPool;
	}

	const EntitySystem::EntityPoolInfo* EntitySystem::findEntityPool( EntityId entityId ) const
	{
		if ( entityId == InvalidEntityId )
		{
			return nullptr;
		}

		const EntityPoolInfo* pEntityPool = nullptr;
		for (uint i = 0u; i < m_pools.getCount(); ++i)
		{
			const EntityPoolInfo& pool = m_pools[ i ];
			if ( entityId >= pool.firstId && entityId < pool.firstId + pool.poolSize )
			{
				pEntityPool = &pool;
				break;
			}
		}

		return pEntityPool;
	}

	EntitySystem::EntityData* EntitySystem::findEntityData( EntityId entityId )
	{
		const EntityPoolInfo* pEntityPool = findEntityPool( entityId );
		if ( pEntityPool == nullptr )
		{
			return nullptr;
		}

		return getEntityDataInPool( pEntityPool, entityId );
	}

	const EntitySystem::EntityData* EntitySystem::findEntityData( EntityId entityId ) const
	{
		const EntityPoolInfo* pEntityPool = findEntityPool( entityId );
		if ( pEntityPool == nullptr )
		{
			return nullptr;
		}

		return getEntityDataInPool( pEntityPool, entityId );
	}

	EntitySystem::EntityData* EntitySystem::getEntityDataInPool( const EntityPoolInfo* pEntityPool, EntityId entityId )
	{
		const uint entityIndex = pEntityPool->offset + ( entityId - pEntityPool->firstId );
		TIKI_ASSERT( m_entities[ entityIndex ].id == entityId || m_entities[ entityIndex ].id == InvalidEntityId );

		return &m_entities[ entityIndex ];
	}

	const EntitySystem::EntityData* EntitySystem::getEntityDataInPool( const EntityPoolInfo* pEntityPool, EntityId entityId ) const
	{
		const uint entityIndex = pEntityPool->offset + ( entityId - pEntityPool->firstId );
		TIKI_ASSERT( m_entities[ entityIndex ].id == entityId || m_entities[ entityIndex ].id == InvalidEntityId );

		return &m_entities[ entityIndex ];
	}
}
