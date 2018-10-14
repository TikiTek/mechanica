#include "tiki/toolapplication/tool_image.hpp"

namespace tiki
{
	ToolImage::ToolImage()
	{
	}

	ToolImage::~ToolImage()
	{
		dispose();
	}

	bool ToolImage::create( uint width, uint height, const Color* pPixels )
	{
		TextureDescription description;
		description.width		= uint16( width );
		description.height		= uint16( height );
		description.depth		= 1u;
		description.flags		= TextureFlags_ShaderInput;
		description.format		= PixelFormat_R8G8B8A8;
		description.type		= TextureType_2d;

		return m_data.create( tool::getGraphicsSystem(), description, pPixels );
	}

	bool ToolImage::createFromFile( const Path& filename )
	{
		return tool::loadImage( m_data, filename );
	}

	bool ToolImage::createFromFile( const char* pFilename )
	{
		return createFromFile( tool::getFile( pFilename ) );
	}

	void ToolImage::dispose()
	{
		m_data.dispose( tool::getGraphicsSystem() );
	}
}
