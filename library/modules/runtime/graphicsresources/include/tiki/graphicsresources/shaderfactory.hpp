#pragma once
#ifndef TIKI_SHADERFACTORY_HPP
#define TIKI_SHADERFACTORY_HPP

#include "tiki/resource/factorybase.hpp"

namespace tiki
{
	class GraphicsSystem;
	
	class ShaderFactory : public FactoryBase
	{
	public:

		ShaderFactory();
		~ShaderFactory();

		void				create( GraphicsSystem& graphicsSystem );
		void				dispose();

		virtual fourcc		getType() const;

		virtual Resource*	createResource() const;
		virtual void		destroyResource( Resource* pResource ) const;

		virtual bool		initializeResource( Resource* pResource, const void* pInitData, uint initDataSize ) const;

	private:

		GraphicsSystem*		m_pGraphicsSystem;

	};
}

#endif // TIKI_SHADERFACTORY_HPP
