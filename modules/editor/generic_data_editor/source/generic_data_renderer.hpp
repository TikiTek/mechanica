#pragma once

#include "tiki/container/list.hpp"
#include "tiki/generic_data_editor/generic_data_view.hpp"
#include "tiki/graphics/immediate_renderer.hpp"
#include "tiki/renderer_2d/renderer_2d.hpp"
#include "tiki/resource/resource_request_pool.hpp"

namespace tiki
{
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

		void							setBaseObject( GenericDataObject* pBaseObject );

		bool							resize( uint16 width, uint16 height );

		void							registerView( GenericDataView& view );
		void							unregisterView( GenericDataView& view );

		const TextureData&				getOutput() const { return m_renderTargetData; }

		bool							handleInputEvent( const InputEvent& inputEvent );

	private:

		typedef Map< const GenericDataTypeStruct*, GenericDataView* > StructViewMap;
		typedef Map< GenericDataObject*, GenericDataViewInfo > ObjectInfoMap;

		GraphicsSystem*					m_pGraphicsSystem;
		ResourceRequestPool				m_resourceRequests;

		uint16							m_width;
		uint16							m_height;
		TextureData						m_renderTargetData;
		RenderTarget					m_renderTarget;
		Renderer2d						m_renderer;
		ImmediateRenderer				m_immediateRenderer;

		GenericDataObject*				m_pBaseObject;
		ObjectInfoMap					m_objectInfos;

		StructViewMap					m_views;
	};
}