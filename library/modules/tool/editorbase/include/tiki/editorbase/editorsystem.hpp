#pragma once
#ifndef TIKI_EDITORSYSTEM_HPP_INCLUDED__
#define TIKI_EDITORSYSTEM_HPP_INCLUDED__

#include "tiki/editorbase/editorcamera.hpp"
#include "tiki/editorbase/editorinput.hpp"

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

		property EditorInput^	Input { EditorInput^ get(); };

	private:

		EditorData*			m_pData;
		TransformGizmo^     m_TransformGizmo;

		EditorInput			m_input;
		EditorCamera		m_camera;

		void				processInputEvent( InputEvent& inputEvent );

	};
}

#endif // TIKI_EDITORSYSTEM_HPP_INCLUDED__
