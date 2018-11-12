#pragma once

#include "tiki/framework/base_application.hpp"

#include "tiki/graphics/color_xkcd.hpp"
#include "tiki/graphics/index_buffer.hpp"
#include "tiki/graphics/texture_data.hpp"
#include "tiki/graphics/vertex_buffer.hpp"

namespace tiki
{
	class ToolApplication;
	class VertexFormat;
	class VertexInputBinding;

	namespace tool
	{
		ToolApplication& getTool();
	}

	struct ToolApplicationParamters : public BaseApplicationParamters
	{
		Color backgroundColor = TIKI_COLOR_XKCD_DARK_NAVY;
	};

	class ToolApplication : public BaseApplication
	{
		TIKI_NONCOPYABLE_CLASS( ToolApplication );

	public:

	protected:

									ToolApplication();
		virtual						~ToolApplication();

	protected:

		virtual void				fillBaseParameters( BaseApplicationParamters& parameters ) TIKI_OVERRIDE_FINAL;

		virtual bool				initializeApplication() TIKI_OVERRIDE_FINAL;
		virtual void				shutdownApplication() TIKI_OVERRIDE_FINAL;

		virtual void				updateApplication( bool wantToShutdown ) TIKI_OVERRIDE_FINAL;
		virtual void				renderApplication( GraphicsContext& graphicsContext ) const TIKI_OVERRIDE_FINAL;

		virtual bool				processBaseInputEvent( const InputEvent& inputEvent ) TIKI_OVERRIDE_FINAL;
		virtual void				processBaseWindowEvent( const WindowEvent& windowEvent ) TIKI_OVERRIDE_FINAL;

		virtual void				fillToolParameters( ToolApplicationParamters& parameters ) TIKI_PURE;
		virtual bool				initializeTool() TIKI_PURE;
		virtual void				shutdownTool() TIKI_PURE;

		virtual void				updateTool( bool wantToShutdown ) TIKI_PURE;
		virtual void				renderTool( GraphicsContext& graphicsContext ) const TIKI_PURE;

		virtual void				doUi() TIKI_PURE;

		virtual bool				processToolInputEvent( const InputEvent& inputEvent ) TIKI_PURE;
		virtual void				processToolWindowEvent( const WindowEvent& windowEvent ) TIKI_PURE;

	private:

		ToolApplicationParamters	m_parameters;

		const VertexFormat*			m_pVertexFormat;
		const VertexInputBinding*	m_pVertexInputBinding;
		IndexBuffer					m_indexBuffer;
		VertexBuffer				m_vertexBuffer;

		TextureData					m_fontTexture;
	};
}
