#pragma once
#ifndef TIKI_SHADERFACTORY_HPP
#define TIKI_SHADERFACTORY_HPP

#include "tiki/resource/factorybase.hpp"

namespace tiki
{
	class GraphicsSystem;
	struct GraphicsHandles;

	class ShaderFactory : public FactoryBase
	{
	public:

							ShaderFactory();
		virtual				~ShaderFactory();

		void				create( GraphicsSystem* pSystem );
		void				dispose();

		virtual fourcc		getType() const;

		virtual Resource*	createResource() const;
		virtual void		destroyResource( Resource* pResource ) const;

		virtual bool		initializeResource( Resource* pResource, const void* pInitData ) const;

	private:

		GraphicsSystem*		m_pGraphicsSystem;
		GraphicsHandles*	m_pGraphicsHandles;

	};
}

#endif // TIKI_SHADERFACTORY_HPP
