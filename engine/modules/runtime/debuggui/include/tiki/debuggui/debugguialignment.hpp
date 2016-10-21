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

		virtual Vector2	getMinimumSize() TIKI_OVERRIDE_FINAL;

		virtual void	update( double elapsedTime ) TIKI_OVERRIDE_FINAL;
		virtual void	render( ImmediateRenderer& renderer ) TIKI_OVERRIDE_FINAL;

		virtual bool	processInputEvent( const InputEvent& inputEvent, const DebugGuiInputState& state ) TIKI_OVERRIDE_FINAL;

	protected:

		virtual void	handleRectangleChanged( const AxisAlignedRectangle& boundingRectangle ) TIKI_OVERRIDE_FINAL;

	private:

		OrientationFlags	m_flags;
		Vector2				m_alignment;

	};
}

#endif // TIKI_DEBUGGUIALIGNMENT_HPP_INCLUDED__
