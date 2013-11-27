#pragma once
#ifndef TIKI_SHADERSETFACTORY_HPP
#define TIKI_SHADERSETFACTORY_HPP

#include "tiki/resource/factorybase.hpp"

namespace tiki
{
	class GraphicsSystem;
	
	class ShaderSetFactory : public FactoryBase
	{
	public:

		ShaderSetFactory();
		~ShaderSetFactory();

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

#endif // TIKI_SHADERSETFACTORY_HPP
