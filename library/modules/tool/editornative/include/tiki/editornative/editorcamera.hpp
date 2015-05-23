#include "tiki\base\types.hpp"

namespace tiki
{
	class Camera;
	class FreeCamera;
	struct Ray3;
	struct InputEvent;

	ref class EditorCamera
	{
	public:

						EditorCamera();

		bool			create();
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