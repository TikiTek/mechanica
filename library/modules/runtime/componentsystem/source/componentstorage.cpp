
#include "tiki/components/componentstorage.hpp"

#include "tiki/base/assert.hpp"

namespace tiki
{
	struct ComponentChunkDefinition
	{

	};

	ComponentStorage::ComponentStorage()
	{
		m_pMemory	= nullptr;
		m_pChunks	= nullptr;
	}

	ComponentStorage::~ComponentStorage()
	{
		TIKI_ASSERT( m_pMemory == nullptr );
		TIKI_ASSERT( m_pChunks == nullptr );
	}

	bool ComponentStorage::create( uint chunkSize, uint chunkCount )
	{
		return false;
	}

	void ComponentStorage::dispose()
	{

	}

	void ComponentStorage::registerComponentType( uint typeId, uint stateSize )
	{

	}

	void ComponentStorage::unregiserComponentType( uint typeId )
	{

	}

	ComponentState* ComponentStorage::allocateState( uint typeId )
	{
		return nullptr;
	}

	void ComponentStorage::freeState( ComponentState* pState )
	{

	}
}