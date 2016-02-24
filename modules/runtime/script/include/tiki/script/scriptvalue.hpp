#pragma once
#ifndef TIKI_SCRIPTVALUE_HPP_INCLUDED
#define TIKI_SCRIPTVALUE_HPP_INCLUDED

#include "tiki/base/types.hpp"

namespace tiki
{
	class ScriptCall;
	class ScriptContext;

	enum ScriptValueType : uint8
	{
		ScriptValueType_Object,
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
						ScriptValue( const ScriptValue& copy );
						~ScriptValue();

		void			createObjectFromStack();
		void			createFloat( double value );
		void			createSigned( sint64 value );
		void			createUnsigned( uint64 value );

		void			dispose();

		void*			getObjectInstanceData() const;
		double			getFloatValue() const;
		sint64			getSignedValue() const;
		uint64			getUnsignedValue() const;

		ScriptValueType	getType() const;
		bool			isObject() const	{ return getType() == ScriptValueType_Object; }
		bool			isFloat() const		{ return getType() == ScriptValueType_Float; }
		bool			isSigned() const	{ return getType() == ScriptValueType_Signed; }
		bool			isUnsigned() const	{ return getType() == ScriptValueType_Unsigned; }

		bool			isValid() const;
		
		ScriptValue&	operator=( const ScriptValue& copy );

		bool			operator==( const ScriptValue& rhs );

	private: // friend

		void			createFromStack( int index );

		void			pushValue() const;

	private:
		
		union Value
		{
			int		objectRef;
			double	floatingPoint;
			sint64	signedInteger;
			uint64	unsignedInteger;
		};

		ScriptContext*	m_pContext;
		ScriptValueType	m_type;
		Value			m_value;

	};
}

#endif // TIKI_SCRIPTVALUE_HPP_INCLUDED
