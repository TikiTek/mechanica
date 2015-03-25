#pragma once
#ifndef TIKI_HTTPRESPONSE_HPP_INCLUDED__
#define TIKI_HTTPRESPONSE_HPP_INCLUDED__

#include "tiki/base/string.hpp"
#include "tiki/webserver/httpstatuscode.hpp"

namespace tiki
{
	struct HttpResponse
	{
		HttpResponse()
		{
			statusCode = HttpStatusCode_500_InternalServerError;
		}

		HttpStatusCode	statusCode;
		string			content;
	};
}

#endif // TIKI_HTTPRESPONSE_HPP_INCLUDED__
