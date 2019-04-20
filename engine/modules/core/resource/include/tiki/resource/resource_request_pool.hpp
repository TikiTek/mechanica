#pragma once

#include "tiki/container/fixed_sized_array.hpp"
#include "tiki/base/types.hpp"

namespace tiki
{
	class Resource;
	class ResourceManager;
	class ResourceRequest;

	class ResourceRequestPool
	{
		TIKI_NONCOPYABLE_CLASS( ResourceRequestPool );

	public:

							ResourceRequestPool();
							~ResourceRequestPool();

		void				create( ResourceManager& resourceManager );
		void				dispose();

		void				update();

		template< typename T >
		void				beginLoadResource( T** ppTargetResource, const char* pFileName );
		template< typename T >
		void				beginLoadResource( const T** ppTargetResource, const char* pFileName );
		template< typename T >
		void				unloadResource( const T*& pResource );

		void				resetError() { m_hasError = false; }

		bool				hasError() const { return m_hasError; }
		bool				isFinish() const { return m_requests.isEmpty(); }

		ResourceManager*	getResourceManager() { return m_pResourceManager; }

	private:

		enum
		{
			MaxResourceRequests = 32u
		};

		struct ResourceRequestData
		{
			const Resource**		ppTargetResource;
			const ResourceRequest*	pRequest;
		};

		typedef FixedSizedArray< ResourceRequestData, MaxResourceRequests > RequestArray;

		ResourceManager*			m_pResourceManager;

		bool						m_hasError;
		RequestArray				m_requests;
	};
}

#include "../../../source/resource_request_pool.inl"
