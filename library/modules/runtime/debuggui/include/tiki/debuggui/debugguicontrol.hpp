#pragma once
#ifndef __TIKI_DEBUGGUICONTROL_HPP_INCLUDED__
#define __TIKI_DEBUGGUICONTROL_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/base/sizedarray.hpp"

namespace tiki
{
	class DebugGuiControl
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( DebugGuiControl );

	public:

		virtual void	setRectangle( const Rectangle& boundingRectangle ) = 0;

		void			addChildControl( DebugGuiControl* pChild );
		bool			removeChildControl( DebugGuiControl* pChild );

		void			update();
		void			render( const ImmediateRenderer& renderer );

	protected:

		virtual void	handleUpdate() = 0;
		virtual void	handleRender( const ImmediateRenderer& renderer ) = 0;

	private:

		SizedArray< DebugGuiControl* >	m_childControls;

	};
}

#endif // __TIKI_DEBUGGUICONTROL_HPP_INCLUDED__
