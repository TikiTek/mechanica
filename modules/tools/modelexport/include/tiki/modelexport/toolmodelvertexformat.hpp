#pragma once
#ifndef TIKI_TOOLMODELVERTEXFORMAT_HPP__INCLUDED
#define TIKI_TOOLMODELVERTEXFORMAT_HPP__INCLUDED

#include "tiki/base/array.hpp"
#include "tiki/base/list.hpp"
#include "tiki/graphicsbase/vertexattribute.hpp"

namespace tiki
{
	class ToolModelVertexFormat
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( ToolModelVertexFormat );

	public:

		void					create( size_t streamCount = 1u );
		void					dispose();

		void					addSemantic( VertexSementic semantic, VertexAttributeFormat format = VertexAttributeFormat_Invalid, size_t streamIndex = 0u );
		bool					hasSemantic( VertexSementic semantic );

		size_t					getVertexStride( size_t streamIndex ) const { return m_vertexStrides[ streamIndex ]; }

		const VertexAttribute*	getAttributes() const { return m_attributes.getData(); }
		size_t					getAttributeCount() const { return m_attributes.getCount(); }

	private:

		Array< size_t >			m_vertexStrides;
		List< VertexAttribute >	m_attributes;

	};
}

#endif // TIKI_TOOLMODELVERTEXFORMAT_HPP__INCLUDED
