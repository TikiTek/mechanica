#pragma once
#ifndef TIKI_SCRIPTVALUE_HPP_INCLUDED
#define TIKI_SCRIPTVALUE_HPP_INCLUDED

#include "tiki/base/types.hpp"

namespace tiki
{
	class ScriptCall;
	class ScriptContext;
	struct ScriptObjectField;

	enum ScriptValueType : uint8
	{
		ScriptValueType_Object,
		ScriptValueType_Function,
		ScriptValueType_Float,
		ScriptValueType_Signed,
		ScriptValueType_Unsigned,

		ScriptValueType_Count,
		ScriptValueType_Invalid = 0xffu
	};

	class ScriptValue
	{
		friend class ScriptCall;
		friend class ScriptContext;

	public:

						ScriptValue();
						ScriptValue( ScriptContext& context );
						ScriptValue( ScriptContext& context, double value );
						ScriptValue( ScriptContext& context, sint64 value );
						ScriptValue( ScriptContext& context, uint64 value );
						ScriptValue( const ScriptValue& copy );
						~ScriptValue();

		void			createReferenceFromStack();
		void			createFloat( double value );
		void			createSigned( sint64 value );
		void			createUnsigned( uint64 value );
		void			createObject( const ScriptObjectField* pFields, uint fieldCount );

		void			dispose();

		void*			getObjectInstanceData() const;
		double			getFloatValue() const;
		sint64			getSignedValue() const;
		uint64			getUnsignedValue() const;

		ScriptValueType	getType() const;
		bool			isObject() const	{ return getType() == ScriptValueType_Object; }
		bool			isFunction() const	{ return getType() == ScriptValueType_Function; }
		bool			isFloat() const		{ return getType() == ScriptValueType_Float; }
		bool			isSigned() const	{ return getType() == ScriptValueType_Signed; }
		bool			isUnsigned() const	{ return getType() == ScriptValueType_Unsigned; }

		bool			isValid() const;

		ScriptValue		callFunction( const ScriptValue& arg1 = ScriptValue(), const ScriptValue& arg2 = ScriptValue(), const ScriptValue& arg3 = ScriptValue() ) const;
		
		ScriptValue&	operator=( const ScriptValue& copy );

		bool			operator==( const ScriptValue& rhs );

	private: // friend

		void			createFromStack( int index );

		void			pushValue() const;

	private:
		
		union Value
		{
			int			objectRef;
			double		floatingPoint;
			sint64		signedInteger;
			uint64		unsignedInteger;
		};

		ScriptContext*	m_pContext;
		ScriptValueType	m_type;
		Value			m_value;
	};

	struct ScriptObjectField
	{
		const char*	pName;
		ScriptValue	value;
	};
}

#endif // TIKI_SCRIPTVALUE_HPP_INCLUDED
