
#include "tiki/graphicsresources/modelgeometry.hpp"

#include "tiki/base/memory.hpp"
#include "tiki/graphics/vertexformat.hpp"
#include "tiki/graphicsresources/material.hpp"

namespace tiki
{
	ModelGeometry::ModelGeometry()
		: m_pVertexFormat( nullptr )
	{
		memory::zero( m_desc );
	}

	const void* ModelGeometry::initialize( GraphicsSystem& graphicsSystem, const void* pData, const Material* pMaterial )
	{
		const uint8* pBinary = (const uint8*)alignPtr( pData, 4u );
		memory::copy( &m_desc, pBinary, sizeof( m_desc ) );

		pBinary = alignPtr( pBinary + sizeof( m_desc ), 4u );
		VertexFormatParameters vertexParams;
		vertexParams.pAttributes	= (const VertexAttribute*)pBinary;
		vertexParams.attributeCount	= m_desc.vertexAttributeCount;

		//m_pVertexFormat				= VertexFormat::getVertexFormat( vertexParams );

		pBinary = alignPtr( pBinary + sizeof( VertexAttribute ) * m_desc.vertexAttributeCount, 16u );
		m_vertexBuffer.create( graphicsSystem, m_desc.vertexCount, m_desc.vertexStride, false, pBinary );
		m_vertexData.create( (const uint8*)pBinary, m_desc.vertexCount * m_desc.vertexStride );

		pBinary = alignPtr( pBinary + m_desc.vertexStride * m_desc.vertexCount, 4u );
		m_indexBuffer.create( graphicsSystem, m_desc.indexCount, (IndexType)m_desc.indexType, false, pBinary );
		m_indexData.create( (const uint32*)pBinary, m_desc.indexCount );

		return pBinary + m_desc.indexType * m_desc.indexCount;
	}

	void ModelGeometry::dispose( GraphicsSystem& graphicsSystem )
	{
		//VertexFormat::releaseVertexFormat( m_pVertexFormat );
		m_pVertexFormat	= nullptr;

		m_vertexBuffer.dispose( graphicsSystem );
		m_indexBuffer.dispose( graphicsSystem );

		m_indexData.dispose();
		m_vertexData.dispose();
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