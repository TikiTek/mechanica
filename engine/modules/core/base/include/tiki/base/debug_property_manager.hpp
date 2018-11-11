#pragma once

#include "tiki/container/linked_list.hpp"

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
