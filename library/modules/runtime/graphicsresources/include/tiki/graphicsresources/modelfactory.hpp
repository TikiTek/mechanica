#ifndef TIKI_MODELFACTORY_HPP__
#define TIKI_MODELFACTORY_HPP__

#include "tiki/resource/factorybase.hpp"

#include "tiki/base/fourcc.hpp"

namespace tiki
{
	class GraphicsSystem;

	class ModelFactory : public FactoryBase
	{
	public:

							ModelFactory();
		virtual				~ModelFactory();

		void				create( GraphicsSystem* pGraphicsSystem );
		void				dispose();
		
		virtual fourcc		getType() const { return TIKI_FOURCC( 'M', 'O', 'D', 'L' ); }

		virtual Resource*	createResource() const;
		virtual void		destroyResource( Resource* pResource ) const;

		virtual bool		initializeResource( Resource* pResource, const void* pInitData ) const;

	private:

		GraphicsSystem*		m_pGraphicsSystem;

	};
}

#endif // TIKI_MODELFACTORY_HPP__
