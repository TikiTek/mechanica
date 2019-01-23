#pragma once

#include "tiki/base/types.hpp"
#include "tiki/math/vector.hpp"

namespace tiki
{
	struct InputEvent;

	class FreeCamera2D
	{
		TIKI_NONCOPYABLE_CLASS( FreeCamera2D );

	public:

						FreeCamera2D();

		void			update( double timeDelta );

		bool			processInputEvent( const InputEvent& inputEvent );

		const Vector2&	getPosition() const { return m_position; }
		float			getZoom() const { return m_zoom; }

	private:

		Vector2		m_position;
		float		m_zoom;
		bool		m_mouseMove;
	};
}
