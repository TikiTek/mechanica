#include "tiki/renderer/renderview.hpp"

#include "tiki/graphics/rendertarget.hpp"

namespace tiki
{
	RenderView::RenderView()
	{
		m_pScene = nullptr;
	}

	RenderView::~RenderView()
	{
		TIKI_ASSERT( m_pScene == nullptr );
	}

	bool RenderView::isCreated() const
	{
		return m_pScene != nullptr;
	}

	void RenderView::create( const RenderScene& scene, const RenderViewParameters& parameters )
	{
		m_pScene = &scene;

		m_fieldOfView	= parameters.cameraFieldOfView;
		m_nearPlane		= parameters.cameraNearPLane;
		m_farPlane		= parameters.cameraFarPLane;

		m_camera.create( parameters.cameraPosition, parameters.cameraRotation );
	}

	void RenderView::dispose()
	{
		m_pScene = nullptr;
	}

	const RenderScene& RenderView::getScene() const
	{
		TIKI_ASSERT( m_pScene != nullptr );
		return *m_pScene;
	}

	Camera& RenderView::getCamera()
	{
		return m_camera;
	}

	const Camera& RenderView::getCamera() const
	{
		return m_camera;
	}

	void RenderView::prepareForRenderTarget( const RenderTarget& renderTarget )
	{
		Projection projection;
		projection.createPerspective(
			(float)renderTarget.getWidth(),
			(float)renderTarget.getHeight(),
			m_fieldOfView,
			m_nearPlane,
			m_farPlane 
		);
		
		m_camera.setProjection( projection );
	}
}