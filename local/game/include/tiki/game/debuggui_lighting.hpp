#pragma once
#ifndef __TIKI_DEBUGGUI_LIGHTING_HPP_INCLUDED__
#define __TIKI_DEBUGGUI_LIGHTING_HPP_INCLUDED__

#include "tiki/debuggui/debugguiwindow.hpp"

#include "tiki/debuggui/debugguibutton.hpp"
#include "tiki/debuggui/debugguiverticallayout.hpp"
#include "tiki/debuggui/debugguicheckbox.hpp"
#include "tiki/debuggui/debugguilabel.hpp"
#include "tiki/debuggui/debugguislider.hpp"

namespace tiki
{
	class RenderScene;

	class DebugGuiLightingWindow : public DebugGuiWindow
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( DebugGuiLightingWindow );

	public:

		void						create( DebugGui& debugGui );
		void						dispose();

		void						fillFrameData( RenderScene& scene );

	private:

		DebugGuiVerticalLayout		m_layout;

		DebugGuiSlider				m_positionX;
		DebugGuiSlider				m_positionY;
		DebugGuiSlider				m_positionZ;

		DebugGuiSlider				m_range;

		DebugGuiSlider				m_theta;
		DebugGuiSlider				m_phi;

	};
}

#endif // __TIKI_DEBUGGUI_LIGHTING_HPP_INCLUDED__
