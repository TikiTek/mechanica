#include "tiki/tool_application/tool_framework.hpp"

#include "tiki/graphics/texture_data.hpp"
#include "tiki/io/directory.hpp"
#include "tiki/io/file.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace tiki
{
	Path tool::getFile( const DynamicString& filename )
	{
		Path path( filename.cStr() );
		if( !file::exists( path.getCompletePath() ) )
		{
			Path executablePath = platform::getExecutablePath();
			executablePath.push( path );

			if( !file::exists( executablePath.getCompletePath() ) )
			{
				return Path();
			}

			return executablePath;
		}

		return path;
	}

	bool tool::loadImage( TextureData& target, ConstMemoryBlock imageData )
	{
		int x;
		int y;
		int comp;
		stbi_uc* pImage = stbi_load_from_memory( (const stbi_uc*)imageData.pStart, (int)imageData.size, &x, &y, &comp, 4u );
		if( pImage == nullptr )
		{
			return false;
		}

		TextureDescription description;
		description.width		= uint16( x );
		description.height		= uint16( y );
		description.depth		= 1u;
		description.flags		= TextureFlags_ShaderInput;
		description.format		= PixelFormat_R8G8B8A8;
		description.type		= TextureType_2d;

		const bool result = target.create( tool::getGraphicsSystem(), description, pImage );
		stbi_image_free( pImage );
		return result;
	}

	bool tool::loadImageFromFile( TextureData& target, const Path& filename )
	{
		int x;
		int y;
		int comp;
		stbi_uc* pImage = stbi_load( filename.getCompletePath(), &x, &y, &comp, 4u );
		if( pImage == nullptr )
		{
			return false;
		}

		TextureDescription description;
		description.width		= uint16( x );
		description.height		= uint16( y );
		description.depth		= 1u;
		description.flags		= TextureFlags_ShaderInput;
		description.format		= PixelFormat_R8G8B8A8;
		description.type		= TextureType_2d;

		const bool result = target.create( tool::getGraphicsSystem(), description, pImage );
		stbi_image_free( pImage );
		return result;
	}

	MainWindow& tool::getMainWindow()
	{
		return getTool().getMainWindow();
	}

	GraphicsSystem& tool::getGraphicsSystem()
	{
		return getTool().getGraphicsSystem();
	}

	ResourceManager& tool::getResourceManager()
	{
		return getTool().getResourceManager();
	}

	InputSystem& tool::getInputSystem()
	{
		return getTool().getInputSystem();
	}

	const Timer& tool::getFrameTimer()
	{
		return getTool().getFrameTimer();
	}

	const ImmediateRenderer& tool::getImmediateRenderer()
	{
		return getTool().getImmediateRenderer();
	}
}
