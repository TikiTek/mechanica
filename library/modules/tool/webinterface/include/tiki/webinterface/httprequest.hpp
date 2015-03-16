#pragma once
#ifndef TIKI_HTTPREQUEST_HPP_INCLUDED__
#define TIKI_HTTPREQUEST_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/webinterface/url.hpp"

namespace tiki
{
	class HttpRequest
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( HttpRequest );

	public:

		bool			create( cstring pUrlString, cstring pBodyString );
		void			dispose();

		const Url&		getUrl() const	{ return m_url; }
		const string&	getBody() const	{ return m_body; }

	private:

		Url				m_url;
		string			m_body;

	};
}

#endif // TIKI_HTTPREQUEST_HPP_INCLUDED__
