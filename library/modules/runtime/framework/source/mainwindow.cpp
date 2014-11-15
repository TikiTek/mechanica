
#include "tiki/framework/mainwindow.hpp"

#include <windows.h>
#include <windowsx.h>

#define GET_HWND (HWND)m_pHandle

namespace tiki
{
	static bool					s_mainWindowCreated	= false;
	static WindowEventBuffer*	s_pEventBuffer		= nullptr;

	LRESULT CALLBACK windowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
	{
		TIKI_ASSERT( s_mainWindowCreated );

		switch (message)
		{
		case WM_CREATE:
			s_pEventBuffer->pushEvent( WindowEventType_Create );
			break;

		case WM_DESTROY:
			PostQuitMessage( 0 );
			s_pEventBuffer->pushEvent( WindowEventType_Destroy );
			break;

		case WM_SIZE:
			{
				RECT rect;
				GetClientRect( hWnd, &rect );

				WindowEvent& event				= s_pEventBuffer->pushEvent( WindowEventType_SizeChanged );
				event.data.sizeChanged.size.x	= (rect.right - rect.left);
				event.data.sizeChanged.size.y	= (rect.bottom - rect.top);
			}
			break;

		case WM_KEYDOWN:
			{
				WindowEvent& event	= s_pEventBuffer->pushEvent( WindowEventType_KeyDown );
				event.data.key.key	= (KeyboardKey)MapVirtualKey( UINT( wParam ), MAPVK_VK_TO_VSC );
			}
			break;

		case WM_KEYUP:
			{
				WindowEvent& event	= s_pEventBuffer->pushEvent( WindowEventType_KeyUp );
				event.data.key.key	= (KeyboardKey)MapVirtualKey( UINT( wParam ), MAPVK_VK_TO_VSC );
			}
			break;

		case WM_MOUSEMOVE:
			{
				WindowEvent& event				= s_pEventBuffer->pushEvent( WindowEventType_MouseMove );
				event.data.mouseMove.position.x	= GET_X_LPARAM( lParam ); 
				event.data.mouseMove.position.y	= GET_Y_LPARAM( lParam ); 
			}
			break;

		case WM_LBUTTONDOWN:
			{
				WindowEvent& event				= s_pEventBuffer->pushEvent( WindowEventType_MouseDown );
				event.data.mouseButton.button	= MouseButton_Left;
			}
			break;

		case WM_MBUTTONDOWN:
			{
				WindowEvent& event				= s_pEventBuffer->pushEvent( WindowEventType_MouseDown );
				event.data.mouseButton.button	= MouseButton_Middle;
			}
			break;

		case WM_RBUTTONDOWN:
			{
				WindowEvent& event				= s_pEventBuffer->pushEvent( WindowEventType_MouseDown );
				event.data.mouseButton.button	= MouseButton_Right;
			}
			break;

		case WM_LBUTTONUP:
			{
				WindowEvent& event				= s_pEventBuffer->pushEvent( WindowEventType_MouseUp );
				event.data.mouseButton.button	= MouseButton_Left;
			}
			break;

		case WM_MBUTTONUP:
			{
				WindowEvent& event				= s_pEventBuffer->pushEvent( WindowEventType_MouseUp );
				event.data.mouseButton.button	= MouseButton_Middle;
			}
			break;

		case WM_RBUTTONUP:
			{
				WindowEvent& event				= s_pEventBuffer->pushEvent( WindowEventType_MouseUp );
				event.data.mouseButton.button	= MouseButton_Right;
			}
			break;

		case WM_TOUCH:
			{
				WindowEvent& event				= s_pEventBuffer->pushEvent( WindowEventType_Touch );
				event.data.touch.pointCount		= LOWORD( wParam );
				event.data.touch.handle			= (TouchInputHandle)lParam;
			}
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		}

		return 0;
	}

	MainWindow::MainWindow()
		: m_pHandle( nullptr )
	{
	}


	bool MainWindow::create( const WindowParameters& params )
	{
		TIKI_ASSERT( s_mainWindowCreated == false );
		s_mainWindowCreated = true;

		HINSTANCE hInst = GetModuleHandle( nullptr );

		TIKI_DECLARE_STACKANDZERO( WNDCLASSEXA, win );
		win.cbSize			= sizeof( WNDCLASSEXA );
		win.hInstance		= hInst;
		win.lpfnWndProc		= &windowProc;
		win.lpszClassName	= params.pClassName;
		win.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);
		win.hCursor			= LoadCursor( NULL, IDC_ARROW );		

		HRESULT r = RegisterClassExA( &win );
		if (FAILED(r))
		{
			MessageBoxA(NULL, "Can't register Class.", params.pWindowTitle, MB_HELP);
			return false;
		}

		m_eventBuffer.create();
		s_pEventBuffer = &m_eventBuffer;

		const HWND hWnd = CreateWindowA(
			params.pClassName,
			params.pWindowTitle,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			int( params.width ),
			int( params.height ),
			NULL,
			NULL,
			hInst,
			NULL
		);

		if ( hWnd == nullptr )
		{
			MessageBoxA( nullptr, "Can't create Window.", params.pWindowTitle, MB_ICONSTOP );
			return false;
		}
		m_pHandle = WindowHandle( hWnd );
		
		TIKI_DECLARE_STACKANDZERO( STARTUPINFO, startupInfo );
		startupInfo.cb			= sizeof(startupInfo);
		startupInfo.dwFlags		= STARTF_USESHOWWINDOW;
		startupInfo.wShowWindow	= SW_SHOWMINNOACTIVE;

		ShowWindow( hWnd, SW_SHOWNORMAL );
		UpdateWindow( hWnd );

		return true;
	}

	void MainWindow::dispose()
	{
		HWND hWnd = GET_HWND;
		CloseWindow( hWnd );
		m_pHandle			= nullptr;

		s_pEventBuffer		= nullptr;
		s_mainWindowCreated	= false;

		m_eventBuffer.dispose();
	}

	void MainWindow::update()
	{
		m_eventBuffer.clear();

		MSG msg;
		if (PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ))
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}

	WindowHandle MainWindow::getHandle() const
	{
		return m_pHandle;
	}

	uint2 MainWindow::getClientSize() const
	{
		HWND hWnd = GET_HWND;

		RECT rect;
		GetClientRect( hWnd, &rect );

		uint2 size =
		{
			uint32( rect.right - rect.left ),
			uint32( rect.bottom - rect.top )
		};

		return size;
	}

	const WindowEventBuffer& MainWindow::getEventBuffer() const
	{
		return m_eventBuffer;
	}

}