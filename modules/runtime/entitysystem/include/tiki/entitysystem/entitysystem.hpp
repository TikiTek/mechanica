#pragma once
#ifndef __TIKI_ENTITYSYSTEM_HPP_INCLUDED__
#define __TIKI_ENTITYSYSTEM_HPP_INCLUDED__

#include "tiki/components/componentstorage.hpp"
#include "tiki/components/typeregister.hpp"

namespace tiki
{
	struct EntityTemplate;

	struct EntitySystemParameters
	{
		uint	typeRegisterMaxCount;
		uint	storageChunkSize;
		uint	storageChunkCount;
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

		const ComponentState*	getFirstComponentOfEntity( EntityId entityId ) const;
		const ComponentState*	getFirstComponentOfEntityAndType( EntityId entityId, ComponentTypeId typeId ) const;
		
	private:

		ComponentTypeRegister	m_typeRegister;
		ComponentStorage		m_storage;
		
	};
}

#endif // __TIKI_ENTITYSYSTEM_HPP_INCLUDED__
