
namespace tiki
{
	class Camera;
	class FreeCamera;
	struct InputEvent;

	ref class EditorCamera
	{
	public:

						EditorCamera();

		bool			create();
		void			dispose();

		void			update( float timeDelta );

		bool			processInputEvent( InputEvent& inputEvent );

		const Camera&	getCamera();

	private:

		Camera*			m_pCamera;
		FreeCamera*		m_pController;

	};
}