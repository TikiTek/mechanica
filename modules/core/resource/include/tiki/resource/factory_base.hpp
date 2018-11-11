#pragma once

#include "tiki/base/types.hpp"

namespace tiki
{
	class Resource;

	class FactoryBase
	{
		public:

			virtual				~FactoryBase();

			virtual fourcc		getType() const = 0;

			virtual Resource*	createResource() const = 0;
			virtual void		destroyResource( Resource* pResource ) const = 0;

			virtual bool		initializeResource( Resource* pResource, const void* pInitData, uint initDataSize ) const = 0;
	};
}
