
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

	void ToolModelVertexFormat::addSemantic( VertexSementic semantic, VertexAttributeFormat format, size_t streamIndex /* = 0u */ )
	{
		TIKI_ASSERT( semantic < VertexSementic_Count);
		TIKI_ASSERT( format < VertexAttributeFormat_Count );
		TIKI_ASSERT( streamIndex < m_vertexStrides.getCount() );

		if ( format == VertexAttributeFormat_Invalid )
		{
			format = getDefaultFormatForSemantic( semantic );
		}

		VertexAttribute& att = m_attributes.add();

		att.semantic		= semantic;
		att.semanticIndex	= 0u;
		att.format			= format;
		att.streamIndex		= streamIndex;
		att.inputType		= VertexInputType_PerVertex;

		m_vertexStrides[ streamIndex ] += getVertexAttributeFormatSize( format );
	}

	bool ToolModelVertexFormat::hasSemantic( VertexSementic semantic )
	{
		for (size_t i = 0u; i < m_attributes.getCount(); ++i)
		{
			if ( m_attributes[ i ].semantic == semantic )
			{
				return true;
			}
		}

		return false;
	}
}