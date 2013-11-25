#pragma once
#ifndef TIKI_TEXTUREFACTORY_HPP
#define TIKI_TEXTUREFACTORY_HPP

#include "tiki/resource/factorybase.hpp"

namespace tiki
{
	class GraphicsSystem;
	
	class TextureFactory : public FactoryBase
	{
	public:

		TextureFactory();
		~TextureFactory();

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

#endif // TIKI_TEXTUREFACTORY_HPP
