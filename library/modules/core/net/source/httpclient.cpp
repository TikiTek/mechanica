
#include "tiki/net/httpclient.hpp"

#include "tiki/net/httprequest.hpp"
#include "tiki/net/httpresponse.hpp"

#include <curl/curl.h>

namespace tiki
{	
	struct HttpData : LinkedItem< HttpData >
	{
		HttpData()
		{
			pHandle = nullptr;
		}

		CURL*			pHandle;

		HttpResponse	response;
	};

	static size_t curlReadUploadCallback( char* pBuffer, size_t size, size_t count, void* pUserData )
	{
		MemoryStream* pStream = (MemoryStream*)pUserData;

		return pStream->read( pBuffer, size * count );
	}

	static size_t curlWriteReceivingCallback( char *pBuffer, size_t size, size_t count, void* pUserData )
	{
		MemoryStream* pStream = (MemoryStream*)pUserData;

		return pStream->write( pBuffer, size * count );
	}

	HttpClient::HttpClient()
	{
		m_pHandle = nullptr;
	}

	HttpClient::~HttpClient()
	{
		TIKI_ASSERT( m_pHandle == nullptr );
	}

	bool HttpClient::create()
	{
		m_pHandle = curl_multi_init();

		return m_pHandle != nullptr;
	}

	void HttpClient::dispose()
	{
		TIKI_ASSERT( m_datas.isEmpty() );

		if ( m_pHandle )
		{
			curl_multi_cleanup( m_pHandle );
			m_pHandle = nullptr;
		}
	}

	void HttpClient::update()
	{
		int runningRequests = 0;
		curl_multi_perform( m_pHandle, &runningRequests );

		CURLMsg* pMessage = nullptr;
		do 
		{
			int dummy = 0;
			pMessage = curl_multi_info_read( m_pHandle, &dummy );
			if ( pMessage != nullptr && pMessage->msg == CURLMSG_DONE )
			{
				HttpData* pData = findDataByHandle( pMessage->easy_handle );
				TIKI_ASSERT( pData != nullptr );

				if ( pData )
				{
					pData->response.isReady	= true;

					if ( pMessage->data.result == CURLE_OK )
					{
						int httpCode = 0;
						curl_easy_getinfo( pData->pHandle, CURLINFO_RESPONSE_CODE, &httpCode );

						pData->response.reponseCode = (HttpCode)httpCode;
					}
					else
					{
						pData->response.reponseCode = HttpCode_BadRequest;
					}

					releaseHandle( *pData );
				}
			}
		}
		while ( pMessage );
	}

	const HttpResponse* HttpClient::startRequest( const HttpRequest& request )
	{
		TIKI_ASSERT( m_pHandle != nullptr );

		HttpData* pData = TIKI_MEMORY_NEW_OBJECT( HttpData );

		pData->pHandle			= curl_easy_init();
		pData->response.request	= request;

		curl_easy_setopt( pData->pHandle, CURLOPT_URL,					request.url.getUrl().cStr() );
		curl_easy_setopt( pData->pHandle, CURLOPT_TIMEOUT,				10 );
		curl_easy_setopt( pData->pHandle, CURLOPT_FOLLOWLOCATION,		1 );
		
		switch ( request.method )
		{
		case HttpMethod_Get:
			// noting to do. get is default
			break;

		case HttpMethod_Post:
			curl_easy_setopt( pData->pHandle, CURLOPT_POST,				1 );
			break;

		case HttpMethod_Put:
			curl_easy_setopt( pData->pHandle, CURLOPT_PUT,				1 );
			break;

		case HttpMethod_Delete:
			curl_easy_setopt( pData->pHandle, CURLOPT_CUSTOMREQUEST,	"DELETE" );
			break;

		default:
			TIKI_BREAK( "case not supported" );
			break;
		}

		curl_easy_setopt( pData->pHandle, CURLOPT_READFUNCTION,			curlReadUploadCallback );
		curl_easy_setopt( pData->pHandle, CURLOPT_READDATA,				&pData->response.request.bodyStream );
		curl_easy_setopt( pData->pHandle, CURLOPT_WRITEFUNCTION,		curlWriteReceivingCallback );
		curl_easy_setopt( pData->pHandle, CURLOPT_WRITEDATA,			&pData->response.bodyStream );

		curl_multi_add_handle( m_pHandle, pData->pHandle );

		m_datas.push( pData );
		return &pData->response;
	}

	void HttpClient::releaseRequest( const HttpResponse* pResponse )
	{
		TIKI_ASSERT( m_pHandle != nullptr );

		HttpData* pData = findDataByResponse( pResponse );
		TIKI_ASSERT( pData != nullptr );

		if ( pData != nullptr )
		{
			m_datas.removeSortedByValue( *pData );
			releaseData( *pData );
		}
	}

	HttpData* HttpClient::findDataByHandle( void* pHandle )
	{
		for ( HttpData& data : m_datas )
		{
			if ( data.pHandle == pHandle )
			{
				return &data;
			}
		}

		return nullptr;
	}

	HttpData* HttpClient::findDataByResponse( const HttpResponse* pResponse )
	{
		for ( HttpData& data : m_datas )
		{
			if ( &data.response == pResponse )
			{
				return &data;
			}
		}

		return nullptr;
	}

	void HttpClient::releaseHandle( HttpData& data )
	{
		if ( data.pHandle )
		{
			curl_multi_remove_handle( m_pHandle, data.pHandle );
			curl_easy_cleanup( data.pHandle );
			data.pHandle = nullptr;
		}
	}

	void HttpClient::releaseData( HttpData& data )
	{
		releaseHandle( data );

		TIKI_MEMORY_DELETE_OBJECT( &data );
	}

}