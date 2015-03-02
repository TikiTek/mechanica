#pragma once
#ifndef TIKI_DEBUGGUIALIGNMENT_HPP_INCLUDED__
#define TIKI_DEBUGGUIALIGNMENT_HPP_INCLUDED__

#include "tiki/debuggui/debugguicontrol.hpp"

namespace tiki
{
	class DebugGuiAlignment : public DebugGuiControl
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( DebugGuiAlignment );

	public:

		enum OrientationFlags
		{
			OrientationFlags_Node	= 0u,
			OrientationFlags_X		= 1u << 0u,
			OrientationFlags_Y		= 1u << 1u
		};

		void			create( OrientationFlags flags, const Vector2& alignment );
		void			dispose();

		virtual Vector2	getMinimumSize();

		virtual void	update();
		virtual void	render( ImmediateRenderer& renderer );

		virtual bool	processInputEvent( const InputEvent& inputEvent, const DebugGuiInputState& state );

	protected:

		virtual void	handleRectangleChanged( const Rectangle& boundingRectangle );

	private:

		OrientationFlags	m_flags;
		Vector2				m_alignment;

	};
}

#endif // TIKI_DEBUGGUIALIGNMENT_HPP_INCLUDED__
