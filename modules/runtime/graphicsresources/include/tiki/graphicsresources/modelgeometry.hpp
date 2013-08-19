#ifndef TIKI_MODELGEOMETRY_HPP__
#define TIKI_MODELGEOMETRY_HPP__

#include "tiki/math/matrix.hpp"

#include "tiki/base/array.hpp"
#include "tiki/graphics/staticbuffer.hpp"

namespace tiki
{
	class Material;
	class VertexFormat;

	struct ModelGeometryDesc
	{
		uint16	vertexCount;
		uint16	indexCount;

		bool	isSkinned;
		uint8	vertexStride;
		uint8	indexSize;		// size per index(16 or 32 bits) - currently not used
		uint8	vertexAttributeCount;
	};

	class ModelGeometry
	{
		friend class Model;

	public:

		ModelGeometry();

		bool						isSkinned() const { return m_desc.isSkinned; }
		
		const VertexFormat*			getVertexFormat() const { return m_pVertexFormat; }

		const StaticBuffer< 1 >&	getVertexBuffer() const { return m_vertexBuffer; }
		const StaticBuffer< 2 >&	getIndexBuffer() const { return m_indexBuffer; }

		const uint8*				getVertexData() const { return m_vertexData.getData(); }
		size_t						getVertexCount() const { return m_vertexData.getCount(); }

		const uint32*				getIndexData() const { return m_indexData.getData(); }
		size_t						getIndexCount() const { return m_indexData.getCount(); }

		void						getMinMax( Vector3& min, Vector3& max ) const;

		const ModelGeometryDesc&	getDesc() const { return m_desc; }

	private:

		const VertexFormat*		m_pVertexFormat;

		ModelGeometryDesc		m_desc;

		Array< uint8 >			m_vertexData;
		Array< uint32 >			m_indexData;

		StaticBuffer< 2 >		m_indexBuffer;
		StaticBuffer< 1 >		m_vertexBuffer;

		const void*				initialize( GraphicsSystem* pGraphicsSystem, const void* pData, const Material* pMaterial );
		void					dispose();

	};
}

#endif // TIKI_MODELGEOMETRY_HPP__
