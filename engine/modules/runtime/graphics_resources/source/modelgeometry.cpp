
#include "tiki/graphics/modelgeometry.hpp"

#include "tiki/base/memory.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphics/vertexformat.hpp"
#include "tiki/graphics/material.hpp"

namespace tiki
{
	ModelGeometry::ModelGeometry()
		: m_pVertexFormat( nullptr )
	{
		memory::zero( m_desc );
	}

	bool ModelGeometry::initialize( GraphicsSystem& graphicsSystem, const ModelGeometryInitData& initData, const Material* pMaterial )
	{
		m_desc = initData.description;

		m_pVertexFormat = graphicsSystem.createVertexFormat(
			initData.vertexAttributes.getData(),
			m_desc.vertexAttributeCount
		);

		m_vertexBuffer.create( graphicsSystem, m_desc.vertexCount, m_desc.vertexStride, false, initData.vertexData.getData() );
		m_vertexData.create( initData.vertexData.getData(), m_desc.vertexCount * m_desc.vertexStride );

		m_indexBuffer.create( graphicsSystem, m_desc.indexCount, (IndexType)m_desc.indexType, false, initData.indexData.getData() );
		m_indexData.create( initData.indexData.getData(), m_desc.indexCount * m_desc.indexType );

		return true;
	}

	void ModelGeometry::dispose( GraphicsSystem& graphicsSystem )
	{
		if ( m_pVertexFormat != nullptr )
		{
			graphicsSystem.disposeVertexFormat( m_pVertexFormat );
			m_pVertexFormat	= nullptr;
		}

		m_vertexBuffer.dispose( graphicsSystem );
		m_indexBuffer.dispose( graphicsSystem );

		m_indexData.dispose();
		m_vertexData.dispose();
	}

	void ModelGeometry::render( GraphicsContext& graphicsContext ) const
	{
		graphicsContext.setVertexBuffer( 0u, m_vertexBuffer );
		graphicsContext.setIndexBuffer( m_indexBuffer );

		graphicsContext.drawIndexedGeometry( m_desc.indexCount );
	}

	//void ModelGeometry::getMinMax( Vector3& min, Vector3& max ) const
	//{
	//	const void* pData = (uint8*)m_vertexData.getData();

	//	for( uint i = 0; i < m_desc.vertexCount; ++i )
	//	{
	//		Vector3 tmp( (float*)pData );

	//		min.min( tmp );
	//		max.max( tmp );

	//		pData = addPtr( pData, m_desc.vertexStride );
	//	}
	//}

}