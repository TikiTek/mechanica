#pragma once
#ifndef TIKI_DEBUGPROP_HPP__INCLUDED
#define TIKI_DEBUGPROP_HPP__INCLUDED

#include "tiki/base/types.hpp"

#if TIKI_ENABLED( TIKI_BUILD_MASTER )

#	define TIKI_DEBUGPROP_BOOL( varname, name, defaultValue ) static const bool varname = defaultValue;
#	define TIKI_DEBUGPROP_FLOAT( varname, name, defaultValue, minValue, maxValue ) static const float varname = defaultValue;

#else

#	define TIKI_DEBUGPROP_BOOL( varname, name, defaultValue ) static ::tiki::DebugPropBool varname##( name, defaultValue );
#	define TIKI_DEBUGPROP_FLOAT( varname, name, defaultValue, minValue, maxValue ) static ::tiki::DebugPropFloat varname##( name, defaultValue, minValue, maxValue );

#endif

namespace tiki
{
	class DebugPropManager;

	class DebugProp
	{
		friend class DebugPropManager;

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
		friend class DebugPropManager;

	public:

		DebugPropBool( cstring pName, const bool defaultValue )
			: DebugProp( pName, TIKI_CURRENT_MODULE, Type_Bool ), m_value( defaultValue ), m_defaultValue( defaultValue )
		{			
		}

		bool		getValue() const		{ return m_value; }
		bool		getDefaultValue() const	{ return m_value; }
		operator	bool() const			{ return m_value; }

	protected:

		void		setValue( const bool value ) { m_value = value; }

	private:

		bool		m_value;
		bool		m_defaultValue;

	};

	class DebugPropFloat : public DebugProp
	{
		friend class DebugPropManager;

	public:

		DebugPropFloat( cstring pName, const float defaultValue, const float minValue, const float maxValue )
			: DebugProp( pName, TIKI_CURRENT_MODULE, Type_Float ), m_value( defaultValue ), m_defaultValue( defaultValue ), m_minValue( minValue ), m_maxValue( maxValue )
		{
		}

		float		getValue() const		{ return m_value; }
		float		getDefaultValue() const	{ return m_value; }
		operator	float() const			{ return m_value; }

	protected:

		void		setValue( const float value ) { m_value = value; }

	private:

		float		m_value;

		float		m_defaultValue;
		float		m_minValue;
		float		m_maxValue;

	};
}

#endif // TIKI_DEBUGPROP_HPP__INCLUDED
