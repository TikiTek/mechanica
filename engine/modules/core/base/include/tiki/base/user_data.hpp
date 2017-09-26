#pragma once
#ifndef TIKI_USER_DATA_HPP_INCLUDED
#define TIKI_USER_DATA_HPP_INCLUDED

namespace tiki
{
	struct UserData
	{
		UserData()
		{
			pContext = nullptr;
			pElement = nullptr;
		}

		UserData( void* _pContext )
		{
			pContext = _pContext;
			pElement = nullptr;
		}

		UserData( void* _pContext, void* _pElement )
		{
			pContext = _pContext;
			pElement = _pElement;
		}

		void*	pContext;
		void*	pElement;
	};
}

#endif // TIKI_USER_DATA_HPP_INCLUDED
