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
							~ToolImage();

		bool				create( uint width, uint height, const Color* pPixels );
		bool				createFromFile( const Path& filename );
		bool				createFromFile( const char* pFilename );
		void				dispose();

		const TextureData&	getData() const { return m_data; }

	private:

		TextureData		m_data;
	};
}