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

		void					loadProperties( const char* pFilename );
		void					saveProperties( const char* pFilename );

		LinkedList<DebugProp>&	getProperties();
	}
}

#endif // TIKI_DEBUGPROPMANAGER_HPP_INCLUDED__
