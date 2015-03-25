#pragma once
#ifndef TIKI_IWEBHANDLER_HPP_INCLUDED__
#define TIKI_IWEBHANDLER_HPP_INCLUDED__

namespace tiki
{
	class IWebHandler
	{
	public:

		virtual cstring	getName() TIKI_PURE;

		virtual bool	handleRequest( string& responseContent, cstring requestedPath ) TIKI_PURE;

	};
}

#endif // TIKI_IWEBHANDLER_HPP_INCLUDED__
