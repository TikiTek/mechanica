#pragma once
#ifndef __TIKI_FILESYSTEM_HPP_INCLUDED__
#define __TIKI_FILESYSTEM_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/container/linkedlist.hpp"

namespace tiki
{
	struct HttpData;
	struct HttpRequest;
	struct HttpResponse;

	class HttpClient
	{
		TIKI_NONCOPYABLE_CLASS( HttpClient );

	public:

								HttpClient();
								~HttpClient();

		bool					create();
		void					dispose();

		void					update();

		const HttpResponse*		startRequest( const HttpRequest& request );
		void					releaseRequest( const HttpResponse* pResponse );

	private:

		void*					m_pHandle;

		LinkedList< HttpData >	m_datas;

		HttpData*				findDataByHandle( void* pHandle );
		HttpData*				findDataByResponse( const HttpResponse* pResponse );

		void					releaseHandle( HttpData& data );
		void					releaseData( HttpData& data );

	};
}

#endif // __TIKI_FILESYSTEM_HPP_INCLUDED__
