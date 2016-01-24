#pragma once
#ifndef TIKI_SCRIPTCONTEXT_HPP_INCLUDED
#define TIKI_SCRIPTCONTEXT_HPP_INCLUDED

#include "tiki/base/types.hpp"
#include "tiki/container/linkedlist.hpp"

struct lua_State;

namespace tiki
{
	class ScriptCallContext;

	typedef void(*ScriptFunc)(ScriptCallContext& context );

	enum ScriptExtensions
	{
		ScriptExtensions_Base,
		ScriptExtensions_Objects,
		ScriptExtensions_IO,
		ScriptExtensions_String,
		ScriptExtensions_Math,

		ScriptExtensions_Count
	};

	class ScriptContext
	{
		TIKI_NONCOPYABLE_CLASS( ScriptContext );

	public:

				ScriptContext();
				~ScriptContext();

		bool	create();
		void	dispose();

		void	addExtension( ScriptExtensions extension );

		void	registerFunction( ScriptFunc* pFunc );
		void	unregisterFunction( ScriptFunc* pFunc );
		
		bool	executeScript( const char* pCode );

	private:

		lua_State*						m_pState;

		static void*					allocateMemory( void* pUserData, void* pPointer, size_t osize, size_t nsize );

	};

}


#endif // TIKI_SCRIPTCONTEXT_HPP_INCLUDED
