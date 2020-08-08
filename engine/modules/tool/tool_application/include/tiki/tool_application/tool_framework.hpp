#pragma once

#include "tiki/base/dynamic_string.hpp"
#include "tiki/base/path.hpp"

namespace tiki
{
	class GraphicsSystem;
	class ImmediateRenderer;
	class InputSystem;
	class MainWindow;
	class ResourceManager;
	class TextureData;
	class Timer;

	namespace tool
	{
		Path							getFile( const DynamicString& filename );

		bool							loadImage( TextureData& target, ConstMemoryBlock imageData );
		bool							loadImageFromFile( TextureData& target, const Path& filename );

		MainWindow&						getMainWindow();
		GraphicsSystem&					getGraphicsSystem();
		ResourceManager&				getResourceManager();
		InputSystem&					getInputSystem();
		const Timer&					getFrameTimer();
		const ImmediateRenderer&		getImmediateRenderer();
	}
}
