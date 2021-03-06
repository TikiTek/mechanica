#pragma once
#ifndef TIKI_SCRIPTCONTEXT_HPP_INCLUDED
#define TIKI_SCRIPTCONTEXT_HPP_INCLUDED

#include "tiki/base/types.hpp"
#include "tiki/container/linkedlist.hpp"
#include "tiki/script/scriptcall.hpp"

struct lua_State;

namespace tiki
{
	class ScriptCall;
	class ScriptClass;
	class ScriptValue;

	typedef void( *ScriptFunc )( const ScriptCall& context );
	typedef int( *ScriptWrapperFunc )( lua_State* pState );

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
		friend class ScriptCall;
		friend class ScriptClass;
		friend class ScriptValue;

	public:

						ScriptContext();
						~ScriptContext();

		bool			create();
		void			dispose();

		void			addExtension( ScriptExtensions extension );

		void			registerFunction( ScriptFunc* pFunc );
		void			unregisterFunction( ScriptFunc* pFunc );
		
		void			setGlobalValue( const char* pVariableName, const ScriptValue& value );

		bool			executeScript( const char* pCode );

		template< ScriptFunc TFunc >
		static int		scriptWrapperFunction( lua_State* pState );

	private:

		lua_State*		m_pState;

		static void*	allocateMemory( void* pUserData, void* pPointer, uintreg osize, uintreg nsize );

		void			stackDump();

	};

}

#include "../../../source/scriptcontext.inl"

#endif // TIKI_SCRIPTCONTEXT_HPP_INCLUDED
