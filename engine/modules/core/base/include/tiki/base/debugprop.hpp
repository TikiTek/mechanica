#pragma once
#ifndef TIKI_DEBUGPROP_HPP__INCLUDED
#define TIKI_DEBUGPROP_HPP__INCLUDED

#include "tiki/base/debugpropmanager.hpp"
#include "tiki/container/linkedlist.hpp"
#include "tiki/base/string.hpp"
#include "tiki/base/types.hpp"

#if TIKI_ENABLED( TIKI_BUILD_MASTER )

#	define TIKI_DEBUGPROP_BOOL( varname, name, defaultValue )						static const bool varname = defaultValue;
#	define TIKI_DEBUGPROP_INT( varname, name, defaultValue, minValue, maxValue )	static const bool varname = defaultValue;
#	define TIKI_DEBUGPROP_FLOAT( varname, name, defaultValue, minValue, maxValue )	static const float varname = defaultValue;

#	define TIKI_DEBUGPROP_IMPORT_BOOL( varname, name )	static extern const bool varname;
#	define TIKI_DEBUGPROP_IMPORT_INT( varname, name )	static extern const bool varname;
#	define TIKI_DEBUGPROP_IMPORT_FLOAT( varname, name )	static extern const float varname;

#else

#	ifndef TIKI_CURRENT_MODULE
#		define TIKI_CURRENT_MODULE TIKI_STRING( TIKI_PROJECT_NAME )
#	endif

#	define TIKI_DEBUGPROP_BOOL( varname, name, defaultValue ) static ::tiki::DebugPropBool varname ( name, TIKI_CURRENT_MODULE, defaultValue );
#	define TIKI_DEBUGPROP_INT( varname, name, defaultValue, minValue, maxValue ) static ::tiki::DebugPropInt varname ( name, TIKI_CURRENT_MODULE, defaultValue, minValue, maxValue );
#	define TIKI_DEBUGPROP_FLOAT( varname, name, defaultValue, minValue, maxValue ) static ::tiki::DebugPropFloat varname ( name, TIKI_CURRENT_MODULE, defaultValue, minValue, maxValue );

#	define TIKI_DEBUGPROP_IMPORT_BOOL( varname, name )	static extern const ::tiki::DebugPropBool varname;
#	define TIKI_DEBUGPROP_IMPORT_INT( varname, name )	static extern const ::tiki::DebugPropInt varname;
#	define TIKI_DEBUGPROP_IMPORT_FLOAT( varname, name )	static extern const ::tiki::DebugPropFloat varname;

#endif

namespace tiki
{
	class DebugProp : public LinkedItem<DebugProp>
	{
		TIKI_NONCOPYABLE_CLASS( DebugProp );

	public:

		enum Type
		{
			Type_Bool,
			Type_Int,
			Type_Float
		};

		DebugProp( const char* pName, const char* pModule, Type type )
			: m_pName( pName ), m_pModule( pModule ), m_type( type )
		{
			debugprop::registerProperty( *this );
		}

		~DebugProp()
		{
			debugprop::unregisterProperty( *this );
		}

		Type			getType() const		{ return m_type; }
		const char*		getFullName() const	{ return m_pName; }
		const char*		getModule() const	{ return m_pModule; }
		const char*		getName() const
		{
			uint index = stringLastIndexOf( m_pName, '/' );
			index = ( index == TIKI_SIZE_T_MAX ? 0u : index + 1u );

			return m_pName + index;
		}

	protected:

		Type			m_type;
		const char*		m_pName;
		const char*		m_pModule;

	};

	class DebugPropBool : public DebugProp
	{
		TIKI_NONCOPYABLE_CLASS( DebugPropBool );

	public:

		DebugPropBool( const char* pName, const char* pModule, bool defaultValue )
			: DebugProp( pName, pModule, Type_Bool ), m_value( defaultValue ), m_defaultValue( defaultValue )
		{
		}

		bool		getValue() const		{ return m_value; }
		bool		getDefaultValue() const	{ return m_value; }
		operator	bool() const			{ return m_value; }

		void		setValue( bool value )	{ m_value = value; }

	private:

		bool		m_value;
		bool		m_defaultValue;

	};

	class DebugPropInt : public DebugProp
	{
		TIKI_NONCOPYABLE_CLASS( DebugPropInt );

	public:

		DebugPropInt( const char* pName, const char* pModule, int defaultValue, int minValue, int maxValue )
			: DebugProp( pName, pModule, Type_Int ), m_value( defaultValue ), m_defaultValue( defaultValue ), m_minValue( minValue ), m_maxValue( maxValue )
		{
			TIKI_ASSERT( defaultValue >= minValue );
			TIKI_ASSERT( defaultValue <= maxValue );
		}

		int			getValue() const		{ return m_value; }
		int			getDefaultValue() const	{ return m_value; }
		operator	int() const				{ return m_value; }

		int			getMinValue() const		{ return m_minValue; }
		int			getMaxValue() const		{ return m_maxValue; }
			
		void		setValue( int value )
		{
			TIKI_ASSERT( value >= m_minValue );
			TIKI_ASSERT( value <= m_maxValue );

			m_value = value;
		}

	private:

		int			m_value;

		int			m_defaultValue;
		int			m_minValue;
		int			m_maxValue;

	};

	class DebugPropFloat : public DebugProp
	{
		TIKI_NONCOPYABLE_CLASS( DebugPropFloat );

	public:

		DebugPropFloat( const char* pName, const char* pModule, float defaultValue, float minValue, float maxValue )
			: DebugProp( pName, pModule, Type_Float ), m_value( defaultValue ), m_defaultValue( defaultValue ), m_minValue( minValue ), m_maxValue( maxValue )
		{
			TIKI_ASSERT( defaultValue >= minValue );
			TIKI_ASSERT( defaultValue <= maxValue );
		}

		float		getValue() const		{ return m_value; }
		float		getDefaultValue() const	{ return m_value; }
		operator	float() const			{ return m_value; }

		float		getMinValue() const		{ return m_minValue; }
		float		getMaxValue() const		{ return m_maxValue; }

		void		setValue( float value )
		{
			TIKI_ASSERT( value >= m_minValue );
			TIKI_ASSERT( value <= m_maxValue );

			m_value = value;
		}

	private:

		float		m_value;

		float		m_defaultValue;
		float		m_minValue;
		float		m_maxValue;

	};
}

#endif // TIKI_DEBUGPROP_HPP__INCLUDED
