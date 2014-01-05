
#include "tiki/modelexport/toolmodelvertexformat.hpp"

namespace tiki
{
	void ToolModelVertexFormat::create( size_t streamCount /* = 1u */ )
	{
		m_vertexStrides.create( streamCount );

		for (size_t i = 0u; i < m_vertexStrides.getCount(); ++i)
		{
			m_vertexStrides[ i ] = 0u;
		}
	}

	void ToolModelVertexFormat::dispose()
	{
		m_vertexStrides.dispose();
		m_attributes.clear();
	}

	bool ToolModelVertexFormat::hasSemantic( VertexSementic semantic ) const
	{
		return findAttributeBySemantic( semantic, 0u ) != nullptr;
	}

	void ToolModelVertexFormat::addSemantic( VertexSementic semantic, VertexAttributeFormat format, size_t streamIndex /* = 0u */ )
	{
		TIKI_ASSERT( semantic < VertexSementic_Count);
		TIKI_ASSERT( format < VertexAttributeFormat_Count );
		TIKI_ASSERT( streamIndex < m_vertexStrides.getCount() );
		setAttribute( m_attributes.add(), semantic, format, streamIndex );
	}

	bool ToolModelVertexFormat::insertSemantic( VertexSementic semantic, VertexSementic afterThisSemantic, VertexAttributeFormat format /*= VertexAttributeFormat_Invalid*/, size_t streamIndex /*= 0u */ )
	{
		TIKI_ASSERT( semantic < VertexSementic_Count);
		TIKI_ASSERT( format < VertexAttributeFormat_Count );
		TIKI_ASSERT( streamIndex < m_vertexStrides.getCount() );

		const VertexAttribute* pAttribute = findAttributeBySemantic( afterThisSemantic, 0u );
		if ( pAttribute == nullptr )
		{
			return false;
		}
		const int index = m_attributes.indexOf( *pAttribute );
		const uint finalIndex = (uint)index + 1u;
		TIKI_ASSERT( index > 0u );

		m_attributes.add();
		for (uint i = m_attributes.getCount() - 1u; i > finalIndex; --i)
		{
			m_attributes[ i ] = m_attributes[ i - 1u ];
		}

		setAttribute( m_attributes[ index + 1u ], semantic, format, streamIndex ); 

		return true;
	}
	
	const VertexAttribute* ToolModelVertexFormat::findAttributeBySemantic( VertexSementic semantic, uint semanicIndex ) const
	{
		for (size_t i = 0u; i < m_attributes.getCount(); ++i)
		{
			const VertexAttribute& attribute = m_attributes[ i ];

			if ( attribute.semantic == semantic && attribute.semanticIndex == semanicIndex )
			{
				return &attribute;
			}
		}

		return nullptr;
	}

	void ToolModelVertexFormat::setAttribute( VertexAttribute& target, VertexSementic semantic, VertexAttributeFormat format, size_t streamIndex )
	{
		if ( format == VertexAttributeFormat_Invalid )
		{
			format = getDefaultFormatForSemantic( semantic );
		}

		target.semantic			= semantic;
		target.semanticIndex	= 0u;
		target.format			= format;
		target.streamIndex		= streamIndex;
		target.inputType		= VertexInputType_PerVertex;

		m_vertexStrides[ streamIndex ] += getVertexAttributeFormatSize( format );
	}
}