#pragma once
#ifndef TIKI_UISYSTEM_HPP_INCLUDED
#define TIKI_UISYSTEM_HPP_INCLUDED

#include "tiki/base/types.hpp"
#include "tiki/container/chunkstorage.hpp"
#include "tiki/ui/uirenderer.hpp"

namespace tiki
{
	class GraphicsContext;
	struct InputEvent;

	struct UiSystemParameters
	{
		uint					maxElementCount;

		UiRendererParameters	rendererParameters;
	};

	class UiSystem
	{
		TIKI_NONCOPYABLE_CLASS( UiSystem );

	public:

						UiSystem();
						~UiSystem();

		bool			create( const UiSystemParameters& parameters );
		void			dispose();

		void			update();
		void			render( GraphicsContext& context ) const;

		bool			processInputEvent( InputEvent& inputEvent );

	private:

		UiRenderer		m_renderer;

		ChunkStorage	m_storage;

	};
}

#endif // TIKI_UISYSTEM_HPP_INCLUDED
