
#include "tiki/graphics/vertexformat.hpp"

#include "tiki/base/types.hpp"

namespace tiki
{
	void VertexFormat::create( const VertexFormatParameters& creationParameters )
	{
		TIKI_ASSERT( creationParameters.attributeCount < m_attributes.getCapacity() );

		for (uint i = 0u; i < creationParameters.attributeCount; ++i)
		{
			const VertexAttribute& attribute = creationParameters.pAttributes[ i ];

			m_attributes.push( attribute );
			while ( attribute.streamIndex < m_vertexStride.getCount() )
			{
				m_vertexStride.push( 0u );
			}

			m_vertexStride[ attribute.streamIndex ] += getVertexAttributeFormatSize( attribute.format );
		} 
	}

	void VertexFormat::dispose()
	{
	}
}