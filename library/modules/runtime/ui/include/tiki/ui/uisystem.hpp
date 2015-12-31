#pragma once
#ifndef TIKI_UISYSTEM_HPP_INCLUDED
#define TIKI_UISYSTEM_HPP_INCLUDED

#include "tiki/container/linkedlist.hpp"
#include "tiki/base/types.hpp"
#include "tiki/container/pool.hpp"
#include "tiki/ui/uirenderer.hpp"

namespace tiki
{
	class GraphicsContext;
	class GraphicsSystem;
	class ResourceManager;
	struct InputEvent;

	struct UiSystemParameters
	{
		UiSystemParameters()
		{
			maxElementCount = 4096u;
		}

		uint					maxElementCount;

		UiRendererParameters	rendererParameters;
	};

	class UiSystem
	{
		TIKI_NONCOPYABLE_CLASS( UiSystem );

	public:

						UiSystem();
						~UiSystem();

		bool			create( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager, const UiSystemParameters& parameters );
		void			dispose( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager );

		UiElement*		addElement( UiElement* pParent = nullptr );
		void			removeElement( UiElement* pElement );

		void			update();
		void			render( GraphicsContext& context ) const;

		bool			processInputEvent( InputEvent& inputEvent );

	private:

		UiRenderer				m_renderer;

		Pool< UiElement >		m_elementPool;
		LinkedList< UiElement >	m_elements;

		void					updateLayout();

	};
}

#endif // TIKI_UISYSTEM_HPP_INCLUDED
