#ifndef TIKI_VERTEXFORMAT_HPP
#define TIKI_VERTEXFORMAT_HPP

#include "tiki/base/fixedarray.hpp"
#include "tiki/base/fixedsizedarray.hpp"
#include "tiki/base/types.hpp"
#include "tiki/graphicsbase/graphicsstateobject.hpp"
#include "tiki/graphicsbase/graphissystemlimits.hpp"
#include "tiki/graphicsbase/vertexattribute.hpp"

namespace tiki
{
	class Shader;

	struct VertexFormatParameters
	{
		const VertexAttribute*	pAttributes;
		size_t					attributeCount;
	};

	class VertexFormat : public GraphicsStateObject
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( VertexFormat );
		friend class GraphicsContext;

	public:

		void		create( const VertexFormatParameters& creationParameters );
		void		dispose();

		uint		getVertexStride( uint streamIndex ) const { return m_vertexStride[ streamIndex ]; }

	private:

		FixedSizedArray< VertexAttribute, GraphicsSystemLimits_MaxVertexAttributes >	m_attributes[ GraphicsSystemLimits_MaxInputStreams ];
		FixedSizedArray< uint, GraphicsSystemLimits_MaxInputStreams >					m_vertexStride;

	};
}

#endif // TIKI_VERTEXFORMAT_HPP