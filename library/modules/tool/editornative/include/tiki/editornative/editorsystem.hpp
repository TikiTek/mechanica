#pragma once
#ifndef TIKI_EDITORSYSTEM_HPP_INCLUDED__
#define TIKI_EDITORSYSTEM_HPP_INCLUDED__

namespace tiki
{
	class GraphicsSystem;
	class ResourceManager;

	using namespace System;

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
	
	private:

		GraphicsSystem*		m_pGraphicsSystem;
		ResourceManager*	m_pResourceManager;

	};
}

#endif // TIKI_EDITORSYSTEM_HPP_INCLUDED__
