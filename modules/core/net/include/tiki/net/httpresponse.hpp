#pragma once
#ifndef TIKI_HTTPRESPONSE_HPP_INCLUDED__
#define TIKI_HTTPRESPONSE_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/net/httprequest.hpp"
#include "tiki/io/memorystream.hpp"

namespace tiki
{
	enum HttpCode
	{
		HttpCode_Continue						= 100,
		HttpCode_SwitchingProtocols				= 101,

		HttpCode_OK								= 200,
		HttpCode_Created						= 201,
		HttpCode_Accepted						= 202,
		HttpCode_NonAuthoritativeInformation	= 203,
		HttpCode_NoContent						= 204,
		HttpCode_ResetContent					= 205,
		HttpCode_PartialContent					= 206,

		HttpCode_MultipleChoices				= 300,
		HttpCode_MovedPermanently				= 301,
		HttpCode_Found							= 302,
		HttpCode_SeeOther						= 303,
		HttpCode_NotModified					= 304,
		HttpCode_UseProxy						= 305,
		HttpCode_TemporaryRedirect				= 307,

		HttpCode_BadRequest						= 400,
		HttpCode_Unauthorized					= 401,
		HttpCode_PaymentRequired				= 402,
		HttpCode_Forbidden						= 403,
		HttpCode_NotFound						= 404,
		HttpCode_MethodNotAllowed				= 405,
		HttpCode_NotAcceptable					= 406,
		HttpCode_ProxyAuthenticationRequired	= 407,
		HttpCode_RequestTimeout					= 408,
		HttpCode_Conflict						= 409,
		HttpCode_Gone							= 410,
		HttpCode_LengthRequired					= 411,
		HttpCode_PreconditionFailed				= 412,
		HttpCode_RequestEntityTooLarge			= 413,
		HttpCode_RequestUriTooLong				= 414,
		HttpCode_UnsupportedMediaType			= 415,
		HttpCode_RequestedRangeNotSatisfiable	= 416,
		HttpCode_ExpectationFailed				= 417,

		HttpCode_InternalServerError			= 500,
		HttpCode_NotImplemented					= 501,
		HttpCode_BadGateway						= 502,
		HttpCode_ServiceUnavailable				= 503,
		HttpCode_GatewayTimeout					= 504,
		HttpCode_HttpVersionNotSupported		= 505
	};

	struct HttpResponse
	{
		HttpResponse()
		{
			isReady		= false;
			reponseCode	= HttpCode_BadRequest;
		}

		HttpRequest		request;

		bool			isReady;
		HttpCode		reponseCode;

		MemoryStream	bodyStream;
	};
}

#endif // TIKI_HTTPRESPONSE_HPP_INCLUDED__
