#pragma once
#ifndef TIKI_TEXTUREDATA_HPP__INCLUDED
#define TIKI_TEXTUREDATA_HPP__INCLUDED

#include "tiki/graphicsbase/texturedescription.hpp"

#if TIKI_ENABLED( TIKI_GRAPHICS_D3D11 )
#	include "win_d3d11/texturedata_d3d11.hpp"
#elif TIKI_ENABLED( TIKI_GRAPHICS_OPENGL4 )
#	include "global_opengl4/texturedata_opengl4.hpp"
#endif

namespace tiki
{
	class GraphicsSystem;

	class TextureData
	{
		TIKI_NONCOPYABLE_CLASS( TextureData );
		friend class GraphicsContext;
		friend class GraphicsSystem;
		friend class RenderTarget;

	public:

		TextureData();
		~TextureData();

		size_t						getWidth() const		{ return m_description.width; }
		size_t						getHeight() const		{ return m_description.height; }

		const TextureDescription&	getDesription() const	{ return m_description; }

	private: // friends

		bool						create( GraphicsSystem& graphicsSystem, const TextureDescription& description, const void* pInitData = nullptr );
		void						dispose();

	private:

		TextureDescription	m_description;
		TextureDataPlatform	m_platformData;

	};
}

#endif // TIKI_TEXTUREDATA_HPP__INCLUDED
