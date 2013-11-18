
namespace tiki
{
	template<typename T>
	PoolAllocator::PoolAllocator()
	{
		m_pPool			= nullptr;
		m_count			= 0u;

		m_pUsageBitmask	= nullptr;
	}

	template<typename T>
	PoolAllocator::~PoolAllocator()
	{
		TIKI_ASSERT( m_pPool == nullptr );
	}

	template<typename T>
	void PoolAllocator::create( uint count, uint alignment /* = TIKI_DEFAULT_ALIGNMENT */ )
	{
		m_pPool = static_cast< T* >( memory::allocAlign( count * sizeof( T ), alignment ) );
		m_count	= count;

		if ( m_pMemory == nullptr )
		{
			TIKI_TRACE_ERROR( "[PoolAllocator] Could not allocator Memory.\n" );
			return false;
		}
	}

	template<typename T>
	void PoolAllocator::dispose()
	{
		TIKI_ASSERT( m_pPool != nullptr );
		memory::freeAlign( m_pPool );
	}

	template<typename T>
	void* PoolAllocator::allocate()
	{
		TIKI_ASSERT( m_pPool != nullptr );
		
		return nullptr;
	}

	template<typename T>
	void PoolAllocator::free( T* pObject )
	{
		TIKI_ASSERT( m_pPool != nullptr );
	}
}