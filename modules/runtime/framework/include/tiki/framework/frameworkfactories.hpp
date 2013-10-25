#pragma once
#ifndef TIKI_FRAMEWORKFACTORIES_HPP
#define TIKI_FRAMEWORKFACTORIES_HPP

//#include "tiki/animation/animationfactory.hpp"
//#include "tiki/gameplay/navmeshfactory.hpp"
#include "tiki/graphicsresources/fontfactory.hpp"
#include "tiki/graphicsresources/materialfactory.hpp"
#include "tiki/graphicsresources/modelfactory.hpp"
#include "tiki/graphicsresources/texturefactory.hpp"

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

		//AnimationFactory		m_animationFactory;
		FontFactory				m_fontFactory;
		//LevelColliderFactory	m_levelcolliderFactory;
		//LevelFactory			m_levelFactory;
		MaterialFactory			m_materialFactory;
		ModelFactory			m_modelFactory;
		//NavMeshFactory		m_navmeshfactory;
		TextureFactory			m_textureFactory;

	};
}

#endif // TIKI_FRAMEWORKFACTORIES_HPP
