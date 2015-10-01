#pragma once
#ifndef TIKI_HTTPREQUEST_HPP_INCLUDED__
#define TIKI_HTTPREQUEST_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/io/memorystream.hpp"
#include "tiki/net/url.hpp"

namespace tiki
{
	enum HttpMethod
	{
		HttpMethod_Get,
		HttpMethod_Post,
		HttpMethod_Put,
		HttpMethod_Delete
	};

	struct HttpRequest
	{
		HttpRequest()
		{
			method = HttpMethod_Get;
		}

		HttpMethod		method;
		Url				url;

		MemoryStream	bodyStream;
	};
}

#endif // TIKI_HTTPREQUEST_HPP_INCLUDED__
