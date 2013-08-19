#ifndef TIKI_MATERIALFACTORY_HPP__
#define TIKI_MATERIALFACTORY_HPP__

#include "tiki/resource/factorybase.hpp"

#include "tiki/base/fourcc.hpp"

namespace tiki
{
	class MaterialFactory : public FactoryBase
	{
	public:
								MaterialFactory();
		virtual					~MaterialFactory();

		void					create();
		void					dispose();

		virtual fourcc			getType() const { return TIKI_FOURCC( 'M', 'A', 'T', 'T' ); }

		virtual Resource*		createResource() const;

		virtual void			destroyResource( Resource* pResource ) const;

		virtual bool			initializeResource( Resource* pResource, const void* pInitData ) const;
	};
}

#endif // TIKI_MATERIALFACTORY_HPP__
