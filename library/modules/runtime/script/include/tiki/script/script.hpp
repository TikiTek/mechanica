#pragma once
#ifndef TIKI_SCRIPT_HPP_INCLUDED
#define TIKI_SCRIPT_HPP_INCLUDED

#include "tiki/base/types.hpp"

namespace tiki
{
	class Script
	{
		TIKI_NONCOPYABLE_CLASS( Script );

	public:

				Script();
				~Script();

		bool	create( const char* pCode );
		void	dispose();
		
	private:

		void*	m_pTest;

	};
}

#endif // TIKI_SCRIPT_HPP_INCLUDED
