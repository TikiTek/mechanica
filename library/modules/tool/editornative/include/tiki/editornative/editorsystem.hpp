#pragma once
#ifndef TIKI_EDITORSYSTEM_HPP_INCLUDED__
#define TIKI_EDITORSYSTEM_HPP_INCLUDED__

#using <PresentationCore.dll>

namespace tiki
{
	class GraphicsSystem;
	class ImmediateRenderer;
	class ResourceManager;
	
	struct EditorData;

	ref class TransformGizmo;


	using namespace System;
	using namespace System::Windows::Input;

	public value struct EditorParameters
	{
		IntPtr	windowHandle;
	};

	public ref class EditorSystem
	{
	public:
							EditorSystem();

		bool				create( EditorParameters^ parameters );
		void				dispose();

		void				update();
		void				render();

		bool				resize(int x, int y);

		void				onKeyDown( KeyEventArgs^ e );
		void				onKeyUp( KeyEventArgs^ e );
		void				onMouseDown( MouseButtonEventArgs^ e );
		void				onMouseUp( MouseButtonEventArgs^ e );
		void				onMouseDoubleClick( MouseButtonEventArgs^ e );
		void				onMouseMove( MouseEventArgs^ e );

	private:

		EditorData*			m_pData;
		TransformGizmo^     m_TransformGizmo;
	};
}

#endif // TIKI_EDITORSYSTEM_HPP_INCLUDED__
