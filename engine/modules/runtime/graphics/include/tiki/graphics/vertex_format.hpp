#pragma once

#include "tiki/base/types.hpp"
#include "tiki/container/fixed_array.hpp"
#include "tiki/container/fixed_sized_array.hpp"
#include "tiki/graphics/graphics_state_object.hpp"
#include "tiki/graphics/graphics_system_limits.hpp"
#include "tiki/graphics/vertex_attribute.hpp"

namespace tiki
{
	class Shader;

	struct VertexFormatParameters
	{
		const VertexAttribute*	pAttributes;
		uintreg					attributeCount;
	};

	class VertexFormat : public GraphicsStateObject
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( VertexFormat );
		friend class GraphicsContext;
		friend class GraphicsSystem;

	public:

		bool					isCreated() const;

		const VertexAttribute&	getAttributeByIndex( uintreg index ) const { return m_attributes[ index ]; }
		uintreg					getAttributeCount() const { return m_attributes.getCount(); }

		uintreg					getVertexStride( uintreg streamIndex ) const { return m_vertexStride[ streamIndex ]; }

	private: // friend

		void					create( const VertexFormatParameters& creationParameters );
		void					dispose();

	private:

		FixedSizedArray< VertexAttribute, GraphicsSystemLimits_MaxVertexAttributes >	m_attributes;
		FixedSizedArray< uintreg, GraphicsSystemLimits_MaxInputStreams >					m_vertexStride;
	};
}
