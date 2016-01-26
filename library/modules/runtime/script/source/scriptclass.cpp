#include "tiki/script/scriptclass.hpp"

#include "tiki/container/fixedsizedarray.hpp"
#include "tiki/script/scriptcontext.hpp"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

class Account
{
public:
	Account( double balance ) { m_balance = balance; }
	void deposit( double amount ) { m_balance += amount; }
	void withdraw( double amount ) { m_balance -= amount; }
	double balance( void ) { return m_balance; }
private:
	double m_balance;
};

namespace tiki
{
	ScriptClass::ScriptClass()
	{
		m_pContext	= nullptr;
		m_pName		= nullptr;
	}

	ScriptClass::~ScriptClass()
	{
		TIKI_ASSERT( m_pContext == nullptr );
	}

	bool ScriptClass::create( ScriptContext& context, const char* pName, const ScriptMethod* pMethods, uint methodCount )
	{
		m_pContext	= &context;
		m_pName		= pName;

		lua_State* pState = context.m_pState;

		lua_newtable( pState );
		int methodtable = lua_gettop( pState );

		luaL_newmetatable( pState, pName );
		int metatable = lua_gettop( pState );

		lua_pushliteral( pState, "__metatable" );
		lua_pushvalue( pState, methodtable );
		lua_settable( pState, metatable );  // hide metatable from Lua getmetatable()

		lua_pushliteral( pState, "__index" );
		lua_pushvalue( pState, methodtable );
		lua_settable( pState, metatable );

		//lua_pushliteral( pState, "__gc" );
		//lua_pushcfunction( pState, gc_account );
		//lua_settable( pState, metatable );

		lua_pop( pState, 1 );  // drop metatable
		
		FixedSizedArray< luaL_Reg, 32u > functions;
		for( uint i = 0u; i < methodCount; ++i )
		{
			luaL_Reg& function = functions.push();
			function.name	= pMethods[ i ].pMethodName;
			function.func	= pMethods[ i ].pMethodFunc;
		}

		luaL_Reg& lastFunction = functions.push();
		lastFunction.name	= nullptr;
		lastFunction.func	= nullptr;
		
		luaL_setfuncs( pState, functions.getBegin(), 0 );  // fill methodtable
		lua_pop( pState, 1 );  // drop methodtable

		//lua_register( pState, pName, create_account );

		return true;
	}

	void ScriptClass::dispose()
	{
		m_pContext = nullptr;
	}

	ScriptInstance ScriptClass::registerInstance( void* pInstance )
	{
		TIKI_ASSERT( m_pContext != nullptr );
		lua_State* pState = m_pContext->m_pState;

		*(void**)lua_newuserdata( pState, sizeof( void* ) ) = pInstance;
		luaL_getmetatable( pState, m_pName );
		lua_setmetatable( pState, -2 );

		return ScriptInstance( *m_pContext );
	}

	void ScriptClass::unregisterInstance( ScriptInstance instance )
	{

	}

}

#define lua_boxpointer(pState,u) \
        (*(void **)(lua_newuserdata(pState, sizeof(void *))) = (u))

#define lua_unboxpointer(pState,i)   (*(void **)(lua_touserdata(pState, i)))

int luaL_typerror( lua_State *pState, int narg, const char *tname ) {
	const char *msg = lua_pushfstring( pState, "%s expected, got %s",
		tname, luaL_typename( pState, narg ) );
	return luaL_argerror( pState, narg, msg );
}

class LuaAccount
{
	static const char className[];
	static const luaL_Reg methods[];

	static Account *checkaccount( lua_State *L, int narg ) {
		luaL_checktype( L, narg, LUA_TUSERDATA );
		void *ud = luaL_checkudata( L, narg, className );
		if( !ud ) luaL_typerror( L, narg, className );
		return *(Account**)ud;  // unbox pointer
	}

	static int create_account( lua_State *L ) {
		double balance = luaL_checknumber( L, 1 );
		Account *a = new Account( balance );
		lua_boxpointer( L, a );
		luaL_getmetatable( L, className );
		lua_setmetatable( L, -2 );
		return 1;
	}
	static int deposit( lua_State *L ) {
		Account *a = checkaccount( L, 1 );
		double amount = luaL_checknumber( L, 2 );
		a->deposit( amount );
		return 0;
	}
	static int withdraw( lua_State *L ) {
		Account *a = checkaccount( L, 1 );
		double amount = luaL_checknumber( L, 2 );
		a->withdraw( amount );
		return 0;
	}
	static int balance( lua_State *L ) {
		Account *a = checkaccount( L, 1 );
		double balance = a->balance();
		lua_pushnumber( L, balance );
		return 1;
	}
	static int gc_account( lua_State *L ) {
		Account *a = (Account*)lua_unboxpointer( L, 1 );
		delete a;
		return 0;
	}

public:
	static void Register( lua_State* L ) {
		lua_newtable( L );                 int methodtable = lua_gettop( L );
		luaL_newmetatable( L, className ); int metatable   = lua_gettop( L );

		lua_pushliteral( L, "__metatable" );
		lua_pushvalue( L, methodtable );
		lua_settable( L, metatable );  // hide metatable from Lua getmetatable()

		lua_pushliteral( L, "__index" );
		lua_pushvalue( L, methodtable );
		lua_settable( L, metatable );

		lua_pushliteral( L, "__gc" );
		lua_pushcfunction( L, gc_account );
		lua_settable( L, metatable );

		lua_pop( L, 1 );  // drop metatable

		luaL_setfuncs( L, methods, 0 );  // fill methodtable
		lua_pop( L, 1 );  // drop methodtable

		lua_register( L, className, create_account );
	}
};

const char LuaAccount::className[] = "Account";

#define method(class, name) {#name, class::name}

const luaL_Reg LuaAccount::methods[] ={
	method( LuaAccount, deposit ),
	method( LuaAccount, withdraw ),
	method( LuaAccount, balance ),
	{ 0,0 }
};

//int main( int argc, char* argv[] )
//{
//	lua_State *L = lua_newstate( nullptr, nullptr );
//
//	luaopen_base( L );
//	luaopen_table( L );
//	luaopen_io( L );
//	luaopen_string( L );
//	luaopen_math( L );
//	luaopen_debug( L );
//
//	LuaAccount::Register( L );
//
//	if( argc > 1 ) luaL_dofile( pState, argv[ 1 ] );
//
//	lua_close( L );
//	return 0;
//}