#pragma once
#ifndef TIKI_TEXTUREDATA_HPP__INCLUDED
#define TIKI_TEXTUREDATA_HPP__INCLUDED

#include "tiki/graphicsbase/texturedescription.hpp"

#include "graphicstypes.hpp"

namespace tiki
{
	class GraphicsSystem;

	class TextureData
	{
		TIKI_NONCOPYABLE_CLASS( TextureData );
		friend class GraphicsContext;
		friend class RenderTarget;

	public:

									TextureData();
									~TextureData();

		bool						create( GraphicsSystem& graphicsSystem, const TextureDescription& description, const void* pInitData = nullptr );
		void						dispose();

		size_t						getWidth() const		{ return m_description.width; }
		size_t						getHeight() const		{ return m_description.height; }

		const TextureDescription&	getDesription() const	{ return m_description; }

	private:

		TextureDescription	m_description;

		union
		{
			TGResource*		m_pResource;
			TGTexture1D*	m_pTexture1d;
			TGTexture2D*	m_pTexture2d;
			TGTexture3D*	m_pTexture3d;
		};
		TGShaderResource*	m_pShaderView;
		
	};
}

#endif // TIKI_TEXTUREDATA_HPP__INCLUDED
