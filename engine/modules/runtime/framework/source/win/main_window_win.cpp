#include "tiki/framework/main_window.hpp"

#include "tiki/base/memory.hpp"
#include "tiki/input/input_system.hpp"

#include <windows.h>

namespace tiki
{
	namespace framework
	{
		static bool				handleWindowMessage( void* pUserData, UINT message, WPARAM wParam, LPARAM lParam );
		static LRESULT CALLBACK	windowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
	}

	static bool framework::handleWindowMessage( void* pUserData, UINT message, WPARAM wParam, LPARAM lParam )
	{
		MainWindow& mainWindow = *(MainWindow*)pUserData;
		return MainWindowPlatform::handleWindowMessage( mainWindow, message, wParam, lParam );
	}

	LRESULT CALLBACK framework::windowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
	{
		MainWindowUserData* pUserData = (MainWindowUserData*)GetWindowLongPtr( hWnd, GWLP_USERDATA );
		if( pUserData == nullptr )
		{
			if( message == WM_CREATE && lParam != 0 )
			{
				const CREATESTRUCT* pCreationParameters = (const CREATESTRUCT*)lParam;
				pUserData = (MainWindowUserData*)pCreationParameters->lpCreateParams;
			}
		}

		if( pUserData != nullptr )
		{
			for( uint i = 0u; i < TIKI_COUNT( pUserData->apHandlerFunction ); ++i )
			{
				if( pUserData->apHandlerFunction[ i ] != nullptr )
				{
					if( pUserData->apHandlerFunction[ i ]( pUserData->apUserData[ i ], message, wParam, lParam ) )
					{
						return 0u;
					}
				}
			}
		}

		return DefWindowProc( hWnd, message, wParam, lParam );
	}

	MainWindow::MainWindow()
	{
	}

	MainWindow::~MainWindow()
	{

	}

	bool MainWindow::create( const WindowParameters& params )
	{
		const HINSTANCE hInstance = (HINSTANCE)params.instanceHandle;

		TIKI_DECLARE_STACKANDZERO( WNDCLASSEXA, win );
		win.cbSize			= sizeof( WNDCLASSEXA );
		win.hInstance		= hInstance;
		win.lpfnWndProc		= &framework::windowProc;
		win.lpszClassName	= "TikiMainWindowClass";
		win.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);
		win.hCursor			= LoadCursor( NULL, IDC_ARROW );

		if( params.iconId != 0 )
		{
			win.hIcon		= LoadIcon( hInstance, MAKEINTRESOURCE( params.iconId ) );
			win.hIconSm		= win.hIcon;
		}

		HRESULT result = RegisterClassExA( &win );
		if( FAILED( result ) )
		{
			MessageBoxA(NULL, "Can't register Class.", params.pWindowTitle, MB_HELP);
			return false;
		}

		m_eventBuffer.create( params.maxWindowEvents );

		m_platformData.userData.apUserData[ MainWindowMesageHandler_Window ]		= this;
		m_platformData.userData.apHandlerFunction[ MainWindowMesageHandler_Window ]	= framework::handleWindowMessage;

		const HWND hWnd = CreateWindowA(
			"TikiMainWindowClass",
			params.pWindowTitle,
			WS_OVERLAPPEDWINDOW,
			int( params.postionX ),
			int( params.postionY ),
			int( params.width ),
			int( params.height ),
			NULL,
			NULL,
			hInstance,
			&m_platformData.userData
		);

		if ( hWnd == nullptr )
		{
			MessageBoxA( nullptr, "Can't create Window.", params.pWindowTitle, MB_ICONSTOP );
			return false;
		}
		m_platformData.windowHandle = hWnd;

		SetWindowLongPtr( hWnd, GWLP_USERDATA, (LONG_PTR)&m_platformData.userData );

		ShowWindow( hWnd, SW_SHOWNORMAL );
		UpdateWindow( hWnd );

		return true;
	}

	void MainWindow::dispose()
	{
		CloseWindow( m_platformData.windowHandle );
		m_platformData.windowHandle	= nullptr;

		m_platformData.userData.apUserData[ MainWindowMesageHandler_Window ]		= nullptr;
		m_platformData.userData.apHandlerFunction[ MainWindowMesageHandler_Window ]	= nullptr;

		m_eventBuffer.dispose();
	}

	void MainWindow::update()
	{
		MSG msg;
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}

	WindowHandle MainWindow::getHandle() const
	{
		return WindowHandle( m_platformData.windowHandle );
	}

	sint2 MainWindow::getPosition() const
	{
		RECT windowRect;
		if( !GetWindowRect( m_platformData.windowHandle, &windowRect ) )
		{
			return { 0u, 0u };
		}

		return { sint32( windowRect.left ), sint32( windowRect.top ) };
	}

	uint2 MainWindow::getSize() const
	{
		RECT windowRect;
		if( !GetWindowRect( m_platformData.windowHandle, &windowRect ) )
		{
			return { 0u, 0u };
		}

		return { uint32( windowRect.right - windowRect.left ), uint32( windowRect.bottom - windowRect.top ) };
	}

	uint2 MainWindow::getClientSize() const
	{
		RECT rect;
		GetClientRect( m_platformData.windowHandle, &rect );

		uint2 size =
		{
			uint32( rect.right - rect.left ),
			uint32( rect.bottom - rect.top )
		};

		return size;
	}

	WindowEventBuffer& MainWindow::getEventBuffer()
	{
		return m_eventBuffer;
	}

	/*static*/ bool MainWindowPlatform::handleWindowMessage( MainWindow& mainWindow, UINT message, WPARAM wParam, LPARAM lParam )
	{
		switch( message )
		{
		case WM_CREATE:
			mainWindow.m_eventBuffer.pushEvent( WindowEventType_Create );
			return true;

		case WM_CLOSE:
			mainWindow.m_eventBuffer.pushEvent( WindowEventType_Close );
			return true;

		case WM_SIZE:
			{
				RECT rect;
				GetClientRect( mainWindow.m_platformData.windowHandle, &rect );

				WindowEvent& event				= mainWindow.m_eventBuffer.pushEvent( WindowEventType_SizeChanged );
				event.data.sizeChanged.size.x	= (rect.right - rect.left);
				event.data.sizeChanged.size.y	= (rect.bottom - rect.top);
			}
			return true;

		default:
			break;
		}

		return false;
	}
}
