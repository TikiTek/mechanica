
#include "tiki/base/queue.hpp"

#using <PresentationCore.dll>
#using <System.Windows.Forms.dll>
#using <System.dll>
#using <WindowsBase.dll>

namespace tiki
{
	using namespace System::Windows::Input;

	struct InputEvent;
	
	public ref class EditorInput
	{
	public:

				EditorInput();

		bool	create();
		void	dispose();

		bool	popInputEvent( InputEvent& targetEvent );

		void	onKeyDown( KeyEventArgs^ e );
		void	onKeyUp( KeyEventArgs^ e );
		void	onMouseDown( System::Windows::Forms::MouseEventArgs^ e );
		void	onMouseUp( System::Windows::Forms::MouseEventArgs^ e );
		void	onMouseDoubleClick( System::Windows::Forms::MouseEventArgs^ e );
		void	onMouseMove( System::Windows::Forms::MouseEventArgs^ e );
		
	private:

		typedef System::Collections::Generic::Dictionary< System::Windows::Input::Key, int > KeyMap;

		KeyMap					m_KeyMapping;
		int						m_LastPositionX;
		int						m_LastPositionY;

		Queue< InputEvent >*	m_pEvents;

	};
}