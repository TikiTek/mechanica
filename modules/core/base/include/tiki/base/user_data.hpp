#pragma once

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
