
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
		m_count = 0u;
	}

	template<typename T>
	T* PoolAllocator::allocate()
	{
		TIKI_ASSERT( m_pPool != nullptr );

		uint usageIndex = 0u;
		while ( countLeadingZeros64( m_pUsageBitmask[ usageIndex ] ) == 63u )
		{
			usageIndex++;
		}
		const uint maskIndex = 63u - countLeadingZeros64( m_pUsageBitmask[ usageIndex ] );
		const uint finalIndex = ( usageIndex * 64u ) + maskIndex;

		if ( finalIndex >= m_count )
		{
			return nullptr;
		}
		
		return &m_pPool[ finalIndex ];
	}

	template<typename T>
	void PoolAllocator::free( T* pObject )
	{
		TIKI_ASSERT( m_pPool != nullptr );

		const uint index = pObject - m_pPool;
		TIKI_ASSERT( index < m_count );

		const uint usageIndex = index / 64u;
		const uint maskIndex = index - ( usageIndex * 64u );

		m_pUsageBitmask[ usageIndex ] &= ~( 1u << maskIndex );
	}
}