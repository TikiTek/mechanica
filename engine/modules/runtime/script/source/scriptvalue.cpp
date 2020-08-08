#include "tiki/script/scriptvalue.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/script/scriptcontext.hpp"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

namespace tiki
{
	ScriptValue::ScriptValue()
	{
		m_pContext		= nullptr;
		m_type			= ScriptValueType_Invalid;
	}

	ScriptValue::ScriptValue( ScriptContext& context )
	{
		m_pContext	= &context;
		m_type		= ScriptValueType_Invalid;
	}

	ScriptValue::ScriptValue( const ScriptValue& copy )
	{
		*this = copy;
	}

	ScriptValue::ScriptValue( ScriptContext& context, double value )
	{
		m_pContext = &context;
		createFloat( value );
	}

	ScriptValue::ScriptValue( ScriptContext& context, sint64 value )
	{
		m_pContext = &context;
		createSigned( value );
	}

	ScriptValue::ScriptValue( ScriptContext& context, uint64 value )
	{
		m_pContext = &context;
		createUnsigned( value );
	}

	ScriptValue::~ScriptValue()
	{
		dispose();
	}

	void ScriptValue::createReferenceFromStack()
	{
		TIKI_ASSERT( m_pContext != nullptr );

		if( lua_isfunction( m_pContext->m_pState, -1 ) )
		{
			m_type = ScriptValueType_Function;
		}
		else
		{
			m_type = ScriptValueType_Object;
		}

		m_value.objectRef	= luaL_ref( m_pContext->m_pState, LUA_REGISTRYINDEX );
		if( m_value.objectRef == LUA_REFNIL )
		{
			m_value.objectRef = LUA_NOREF;
		}
	}

	void ScriptValue::createFloat( double value )
	{
		TIKI_ASSERT( m_pContext != nullptr );
		m_type					= ScriptValueType_Float;
		m_value.floatingPoint	= value;
	}

	void ScriptValue::createSigned( sint64 value )
	{
		TIKI_ASSERT( m_pContext != nullptr );
		m_type					= ScriptValueType_Signed;
		m_value.signedInteger	= value;
	}

	void ScriptValue::createUnsigned( uint64 value )
	{
		TIKI_ASSERT( m_pContext != nullptr );
		m_type					= ScriptValueType_Unsigned;
		m_value.unsignedInteger	= value;
	}

	void ScriptValue::createObject( const ScriptObjectField* pFields, uint fieldCount )
	{
		TIKI_ASSERT( m_pContext != nullptr );
		m_type = ScriptValueType_Object;

		lua_createtable( m_pContext->m_pState, 0, (int)fieldCount );

		for( uint i = 0u; i < fieldCount; ++i )
		{
			const ScriptObjectField& field = pFields[ i ];

			lua_pushstring( m_pContext->m_pState, field.pName );
			field.value.pushValue();
			lua_settable( m_pContext->m_pState, -3 );
		}

		createReferenceFromStack();
	}

	void ScriptValue::dispose()
	{
		if( m_type == ScriptValueType_Object && m_value.objectRef != LUA_NOREF )
		{
			TIKI_ASSERT( m_pContext != nullptr );

			luaL_unref( m_pContext->m_pState, m_value.objectRef, LUA_REGISTRYINDEX );
			m_value.objectRef = LUA_NOREF;
		}

		m_type		= ScriptValueType_Invalid;
		m_pContext	= nullptr;
	}

	void* ScriptValue::getObjectInstanceData() const
	{
		TIKI_ASSERT( isValid() && isObject() );

		pushValue();
		if( lua_isuserdata( m_pContext->m_pState, -1 ) )
		{
			void* pUserData = lua_touserdata( m_pContext->m_pState, -1 );
			if( pUserData == nullptr )
			{
				return nullptr;
			}

			return pUserData;
		}

		return nullptr;
	}

	double ScriptValue::getFloatValue() const
	{
		TIKI_ASSERT( isValid() && isFloat() );
		return m_value.floatingPoint;
	}

	sint64 ScriptValue::getSignedValue() const
	{
		TIKI_ASSERT( isValid() && (isSigned() || isFloat()) );
		return isFloat() ? (sint64)m_value.floatingPoint : m_value.signedInteger;
	}

	uint64 ScriptValue::getUnsignedValue() const
	{
		TIKI_ASSERT( isValid() && (isUnsigned() || isFloat()) );
		return isFloat() ? (uint64)m_value.floatingPoint : m_value.unsignedInteger;
	}

	ScriptValueType ScriptValue::getType() const
	{
		return m_type;
	}

	bool ScriptValue::isValid() const
	{
		return m_pContext != nullptr && m_type != ScriptValueType_Invalid;
	}

	ScriptValue ScriptValue::callFunction( const ScriptValue& arg1 /*= ScriptValue()*/, const ScriptValue& arg2 /*= ScriptValue()*/, const ScriptValue& arg3 /*= ScriptValue() */ ) const
	{
		TIKI_ASSERT( isFunction() );

		return ScriptValue();
	}

	ScriptValue& ScriptValue::operator=( const ScriptValue& copy )
	{
		if( copy.isValid() )
		{
			m_pContext	= copy.m_pContext;
			m_type		= copy.m_type;

			switch( m_type )
			{
			case ScriptValueType_Object:
			case ScriptValueType_Function:
				copy.pushValue();
				createReferenceFromStack();
				break;

			case ScriptValueType_Float:
				m_value.floatingPoint = copy.m_value.floatingPoint;
				break;

			case ScriptValueType_Signed:
				m_value.signedInteger = copy.m_value.signedInteger;
				break;

			case ScriptValueType_Unsigned:
				m_value.unsignedInteger = copy.m_value.unsignedInteger;
				break;

			default:
				break;
			}
		}
		else
		{
			m_pContext	= nullptr;
			m_type		= ScriptValueType_Invalid;
		}

		return *this;
	}

	bool ScriptValue::operator==( const ScriptValue& rhs )
	{
		if( m_pContext != rhs.m_pContext || m_type != rhs.m_type )
		{
			return false;
		}

		switch( m_type )
		{
		case ScriptValueType_Object:
		case ScriptValueType_Function:
			return m_value.objectRef == rhs.m_value.objectRef;

		case ScriptValueType_Float:
			return m_value.floatingPoint == rhs.m_value.floatingPoint;

		case ScriptValueType_Signed:
			return m_value.signedInteger == rhs.m_value.signedInteger;

		case ScriptValueType_Unsigned:
			return m_value.unsignedInteger == rhs.m_value.unsignedInteger;

		default:
			break;
		}

		return true;
	}

	void ScriptValue::createFromStack( int index )
	{
		TIKI_ASSERT( m_pContext != nullptr );

		if( lua_gettop( m_pContext->m_pState ) == 0 )
		{
			m_type = ScriptValueType_Invalid;
			return;
		}

		const int type = lua_type( m_pContext->m_pState, index );
		switch( type )
		{
		case LUA_TUSERDATA:
		case LUA_TTABLE:
		case LUA_TFUNCTION:
			createReferenceFromStack();
			break;

		case LUA_TNUMBER:
			createFloat( lua_tonumber( m_pContext->m_pState, index ) );
			break;

		default:
			TIKI_TRACE_WARNING( "[script] Unsupported argument type: %s", lua_typename( m_pContext->m_pState, type ) );
			break;
		}
	}

	void ScriptValue::pushValue() const
	{
		TIKI_ASSERT( isValid() );

		switch( m_type )
		{
		case ScriptValueType_Object:
		case ScriptValueType_Function:
			if( m_value.objectRef == LUA_NOREF )
			{
				lua_pushnil( m_pContext->m_pState );
			}
			else
			{
				lua_rawgeti( m_pContext->m_pState, LUA_REGISTRYINDEX, m_value.objectRef );
			}
			break;

		case ScriptValueType_Float:
			lua_pushnumber( m_pContext->m_pState, (lua_Number)m_value.floatingPoint );
			break;

		case ScriptValueType_Signed:
		case ScriptValueType_Unsigned:
			lua_pushinteger( m_pContext->m_pState, (lua_Integer)m_value.signedInteger );
			break;

		default:
			break;
		}
	}
}