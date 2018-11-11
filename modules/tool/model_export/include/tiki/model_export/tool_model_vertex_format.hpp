#pragma once

#include "tiki/container/array.hpp"
#include "tiki/container/list.hpp"
#include "tiki/graphics/vertex_attribute.hpp"

namespace tiki
{
	TIKI_FORCE_INLINE bool operator==( const VertexAttribute& lhs, const VertexAttribute& rhs )
	{
		return lhs.semantic == rhs.semantic && lhs.semanticIndex == rhs.semanticIndex;
	}

	class ToolModelVertexFormat
	{
	public:

		void					create( uint streamCount = 1u );
		void					dispose();

		bool					hasSemantic( VertexSementic semantic ) const;

		void					addSemantic( VertexSementic semantic, VertexAttributeFormat format = VertexAttributeFormat_Invalid, uint32 streamIndex = 0u );
		bool					insertSemantic( VertexSementic semantic, VertexSementic afterThisSemantic, VertexAttributeFormat format = VertexAttributeFormat_Invalid, uint32 streamIndex = 0u );

		uint					getVertexStride( uint streamIndex ) const { return m_vertexStrides[ streamIndex ]; }

		const VertexAttribute*	getAttributes() const { return m_attributes.getBegin(); }
		uint					getAttributeCount() const { return m_attributes.getCount(); }

	private:

		List< uint32 >			m_vertexStrides;
		List< VertexAttribute >	m_attributes;

		const VertexAttribute*	findAttributeBySemantic( VertexSementic semantic, uint semanicIndex ) const;
		void					setAttribute( VertexAttribute& target, VertexSementic semantic, VertexAttributeFormat format, uint32 streamIndex );
	};
}
