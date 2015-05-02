
#include "tiki/runtimeshared/frameworkfactories.hpp"

#include "tiki/animation/animation.hpp"
#include "tiki/graphics/font.hpp"
#include "tiki/graphics/material.hpp"
#include "tiki/graphics/model.hpp"
#include "tiki/graphics/shaderset.hpp"
#include "tiki/graphics/texture.hpp"

namespace tiki
{
	void FrameworkFactories::create( ResourceManager& resourceManager, GraphicsSystem& graphicsSystem )
	{
		Animation::registerResourceType( resourceManager );
		Font::registerResourceType( resourceManager, graphicsSystem );
		Material::registerResourceType( resourceManager );
		Model::registerResourceType( resourceManager, graphicsSystem );
		ShaderSet::registerResourceType( resourceManager, graphicsSystem );
		Texture::registerResourceType( resourceManager, graphicsSystem );
	}

	void FrameworkFactories::dispose( ResourceManager& resourceManager )
	{
		Animation::unregisterResourceType( resourceManager );
		Font::unregisterResourceType( resourceManager );
		Material::unregisterResourceType( resourceManager );
		Model::unregisterResourceType( resourceManager );
		ShaderSet::unregisterResourceType( resourceManager );
		Texture::unregisterResourceType( resourceManager );
	}
}