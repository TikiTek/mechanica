#pragma once

#include "tiki/container/list.hpp"
#include "tiki/renderer_2d/renderer_2d.hpp"
#include "tiki/resource/resource_request_pool.hpp"

namespace tiki
{
	class GenericDataTypeStruct;
	class GenericDataView;
	class GraphicsContext;
	struct GenericDataViewState;
	struct InputEvent;

	class GenericDataRenderer : public NonCopyable
	{
	public:

										GenericDataRenderer();
										~GenericDataRenderer();

		bool							create( ResourceManager& resourceManager, GraphicsSystem& graphicsSystem );
		void							dispose();

		void							update( float deltaTime );
		void							render( GraphicsContext& graphicsContext );

		bool							resize( uint16 width, uint16 height );

		void							registerView( const GenericDataTypeStruct* pType, GenericDataView* pView );
		void							unregisterView( const GenericDataTypeStruct* pType );

		const TextureData&				getOutput() const { return m_renderTargetData; }

		bool							handleInputEvent( const InputEvent& inputEvent );

	private:

		typedef Map< const GenericDataTypeStruct*, GenericDataView* > StructViewMap;

		GraphicsSystem*					m_pGraphicsSystem;
		ResourceRequestPool				m_resourceRequests;

		uint16							m_width;
		uint16							m_height;
		TextureData						m_renderTargetData;
		RenderTarget					m_renderTarget;
		Renderer2d						m_renderer;

		StructViewMap					m_views;
		List< GenericDataViewState* >	m_viewStates;
	};
}