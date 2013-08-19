#pragma once
#ifndef TIKI_FONTFACTORY_HPP__INCLUDED
#define TIKI_FONTFACTORY_HPP__INCLUDED

#include "tiki/resource/factorybase.hpp"

namespace tiki
{
	class GraphicsSystem;

	class FontFactory : public FactoryBase
	{
	public:

							FontFactory();
		virtual				~FontFactory();

		void				create( GraphicsSystem& graphicsSystem );
		void				dispose();

		virtual fourcc		getType() const;

		virtual Resource*	createResource() const;
		virtual void		destroyResource( Resource* pResource ) const;

		virtual bool		initializeResource( Resource* pResource, const void* pInitData ) const;

	private:

		GraphicsSystem*		m_pGraphicsSystem;

	};
}

#endif // TIKI_FONTFACTORY_HPP__INCLUDED
