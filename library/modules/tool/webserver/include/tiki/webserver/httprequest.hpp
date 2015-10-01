#pragma once
#ifndef TIKI_HTTPREQUEST_HPP_INCLUDED__
#define TIKI_HTTPREQUEST_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/net/url.hpp"

namespace tiki
{
	struct HttpRequest
	{
		Url				url;
		cstring			pBody;
	};
}

#endif // TIKI_HTTPREQUEST_HPP_INCLUDED__
