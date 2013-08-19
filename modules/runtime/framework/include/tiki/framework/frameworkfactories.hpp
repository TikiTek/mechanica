#pragma once
#ifndef TIKI_FRAMEWORKFACTORIES_HPP
#define TIKI_FRAMEWORKFACTORIES_HPP

#include "tiki/animation/animationfactory.hpp"
#include "tiki/gamecore/levelcolliderfactory.hpp"
#include "tiki/gameplay/navmeshfactory.hpp"
#include "tiki/graphics/fontfactory.hpp"
#include "tiki/graphics/levelfactory.hpp"
#include "tiki/graphics/materialfactory.hpp"
#include "tiki/graphics/modelfactory.hpp"
#include "tiki/graphics/shaderfactory.hpp"
#include "tiki/graphics/texturefactory.hpp"

namespace tiki
{
	class GraphicsSystem;
	class ResourceManager;

	class FrameworkFactories
	{
	public:

								FrameworkFactories();
								~FrameworkFactories();

		void					create( ResourceManager& resourceManager, GraphicsSystem& graphicsSystem );
		void					dispose();

	private:

		ResourceManager*		m_pResourceManager;

		AnimationFactory		m_animationFactory;
		FontFactory				m_fontFactory;
		LevelColliderFactory	m_levelcolliderFactory;
		LevelFactory			m_levelFactory;
		MaterialFactory			m_materialFactory;
		ModelFactory			m_modelFactory;
		NavMeshFactory			m_navmeshfactory;
		ShaderFactory			m_shaderFactory;
		TextureFactory			m_textureFactory;

	};
}

#endif // TIKI_FRAMEWORKFACTORIES_HPP
