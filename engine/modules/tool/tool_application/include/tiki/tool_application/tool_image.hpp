#pragma  once

#include "tiki/graphics/color.hpp"
#include "tiki/graphics/texturedata.hpp"

namespace tiki
{
	class Path;

	class ToolImage
	{
	public:

							ToolImage();
		explicit			ToolImage( const char* pFilename );
		explicit			ToolImage( ConstMemoryBlock imageData );
							~ToolImage();

		bool				create( uint width, uint height, const Color* pPixels );
		bool				createFromFile( const Path& filename );
		bool				createFromFile( const char* pFilename );
		bool				createFromMemory( ConstMemoryBlock imageData );
		void				dispose();

		Vector2				getSize() const { return vector::create( float( m_data.getWidth() ), float( m_data.getHeight() ) ); }
		const TextureData&	getData() const { return m_data; }

	private:

		TextureData		m_data;
	};
}