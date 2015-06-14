#include "tiki\base\types.hpp"

namespace tiki
{
	class Camera;
	class FreeCamera;
	class GraphicsSystem;
	struct InputEvent;
	struct Ray3;

	public ref class EditorCamera
	{
	public:

						EditorCamera();

		bool			create( GraphicsSystem& graphicsSystem );
		void			dispose();

		void			update( float timeDelta );

		void			resize( int x, int y );

		bool			processInputEvent( InputEvent& inputEvent );

		const Camera&	getCamera();
		const Ray3&		getMouseRay();

	private:

		Camera*			m_pCamera;
		FreeCamera*		m_pController;

		int				m_ViewportWidth;
		int				m_ViewportHeight;

		sint16			m_MousePosX;
		sint16			m_MousePosY;

		Ray3*			m_Ray;

	};
}