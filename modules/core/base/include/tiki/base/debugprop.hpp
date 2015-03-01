#pragma once
#ifndef TIKI_DEBUGPROP_HPP__INCLUDED
#define TIKI_DEBUGPROP_HPP__INCLUDED

#include "tiki/base/debugpropmanager.hpp"
#include "tiki/base/linkedlist.hpp"
#include "tiki/base/types.hpp"

#if TIKI_ENABLED( TIKI_BUILD_MASTER )

#	define TIKI_DEBUGPROP_BOOL( varname, name, defaultValue )						static const bool varname = defaultValue;
#	define TIKI_DEBUGPROP_INT( varname, name, defaultValue, minValue, maxValue )	static const bool varname = defaultValue;
#	define TIKI_DEBUGPROP_FLOAT( varname, name, defaultValue, minValue, maxValue )	static const float varname = defaultValue;

#	define TIKI_DEBUGPROP_IMPORT_BOOL( varname, name )	static extern const bool varname;
#	define TIKI_DEBUGPROP_IMPORT_INT( varname, name )	static extern const bool varname;
#	define TIKI_DEBUGPROP_IMPORT_FLOAT( varname, name )	static extern const float varname;

#else

#	define TIKI_DEBUGPROP_BOOL( varname, name, defaultValue ) static ::tiki::DebugPropBool varname ( name, defaultValue );
#	define TIKI_DEBUGPROP_INT( varname, name, defaultValue, minValue, maxValue ) static ::tiki::DebugPropInt varname ( name, defaultValue, minValue, maxValue );
#	define TIKI_DEBUGPROP_FLOAT( varname, name, defaultValue, minValue, maxValue ) static ::tiki::DebugPropFloat varname ( name, defaultValue, minValue, maxValue );

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

		DebugProp( cstring pName, cstring pModule, Type type )
			: m_pName( pName ), m_pModule( pModule ), m_type( type )
		{
		}

		Type		getType() const		{ return m_type; }
		cstring		getName() const		{ return m_pName; }
		cstring		getModule() const	{ return m_pModule; }

	protected:

		Type		m_type;
		cstring		m_pName;
		cstring		m_pModule;

	};

	class DebugPropBool : public DebugProp
	{
		TIKI_NONCOPYABLE_CLASS( DebugPropBool );

	public:

		DebugPropBool( cstring pName, bool defaultValue )
			: DebugProp( pName, TIKI_CURRENT_MODULE, Type_Bool ), m_value( defaultValue ), m_defaultValue( defaultValue )
		{
			debugprop::registerProperty( *this );
		}

		~DebugPropBool()
		{
			debugprop::unregisterProperty( *this );
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

		DebugPropInt( cstring pName, int defaultValue, int minValue, int maxValue )
			: DebugProp( pName, TIKI_CURRENT_MODULE, Type_Int ), m_value( defaultValue ), m_defaultValue( defaultValue ), m_minValue( minValue ), m_maxValue( maxValue )
		{
		}

		int			getValue() const		{ return m_value; }
		int			getDefaultValue() const	{ return m_value; }
		operator	int() const				{ return m_value; }

		int			getMinValue() const		{ return m_minValue; }
		int			getMaxValue() const		{ return m_maxValue; }
			
		void		setValue( int value )	{ m_value = value; }

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

		DebugPropFloat( cstring pName, float defaultValue, float minValue, float maxValue )
			: DebugProp( pName, TIKI_CURRENT_MODULE, Type_Float ), m_value( defaultValue ), m_defaultValue( defaultValue ), m_minValue( minValue ), m_maxValue( maxValue )
		{
		}

		float		getValue() const		{ return m_value; }
		float		getDefaultValue() const	{ return m_value; }
		operator	float() const			{ return m_value; }

		float		getMinValue() const		{ return m_minValue; }
		float		getMaxValue() const		{ return m_maxValue; }

		void		setValue( float value ) { m_value = value; }

	private:

		float		m_value;

		float		m_defaultValue;
		float		m_minValue;
		float		m_maxValue;

	};
}

#endif // TIKI_DEBUGPROP_HPP__INCLUDED
