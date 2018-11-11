#include "tiki/resource/resource_request_pool.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/resource/resource_manager.hpp"
#include "tiki/resource/resource_request.hpp"

namespace tiki
{
	ResourceRequestPool::ResourceRequestPool()
	{
		m_pResourceManager	= nullptr;

		m_hasError			= false;
	}

	ResourceRequestPool::~ResourceRequestPool()
	{
		TIKI_ASSERT( m_pResourceManager == nullptr );
	}

	void ResourceRequestPool::create( ResourceManager& resourceManager )
	{
		m_pResourceManager = &resourceManager;
	}

	void ResourceRequestPool::dispose()
	{
		TIKI_ASSERT( isFinish() );

		m_pResourceManager = nullptr;
	}

	void ResourceRequestPool::update()
	{
		for (uint i = 0u; i < m_requests.getCount(); ++i)
		{
			ResourceRequestData& data = m_requests[ i ];

			if ( !data.pRequest->isLoading() )
			{
				if ( data.pRequest->isSuccessful() )
				{
					*data.ppTargetResource = data.pRequest->getResource< Resource >();
				}
				else
				{
					m_hasError = true;
				}

				m_pResourceManager->endResourceLoading( *data.pRequest );

				m_requests.removeUnsortedByIndex( i );
				i--;
			}
		}
	}
}