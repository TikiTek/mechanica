#pragma once
#ifndef TIKI_DEBUGPROPMANAGER_HPP_INCLUDED__
#define TIKI_DEBUGPROPMANAGER_HPP_INCLUDED__

#include "tiki/base/linkedlist.hpp"

namespace tiki
{
	class DebugProp;

	namespace debugprop
	{
		void					registerProperty( DebugProp& prop );
		void					unregisterProperty( DebugProp& prop );

		LinkedList<DebugProp>&	getProperties();
	}

	class DebugPropManager
	{
		TIKI_NONCOPYABLE_CLASS( DebugPropManager );

	public:

		DebugPropManager();
		~DebugPropManager();

		void					registerProperty( DebugProp& prop );
		void					unregisterProperty( DebugProp& prop );

		LinkedList<DebugProp>&	getProperties() { return m_properties; }
		
	private:

		LinkedList< DebugProp >	m_properties;

	};
}

#endif // TIKI_DEBUGPROPMANAGER_HPP_INCLUDED__
