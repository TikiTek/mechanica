#ifndef TIKI_MODELGEOMETRY_HPP__
#define TIKI_MODELGEOMETRY_HPP__

#include "tiki/base/staticarray.hpp"
#include "tiki/graphics/indexbuffer.hpp"
#include "tiki/graphics/vertexbuffer.hpp"
#include "tiki/graphicsbase/vertexattribute.hpp"
#include "tiki/math/matrix.hpp"
#include "tiki/resource/resourcefile.hpp"

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
		uint8	indexType;		// see IndexType
		uint8	vertexAttributeCount;
	};

	struct ModelGeometryInitData
	{
		ModelGeometryDesc			description;
		ResRef< VertexAttribute >	vertexAttributes;

		ResRef< uint8 >				vertexData;
		ResRef< uint16 >			indexData;
	};

	class ModelGeometry
	{
		friend class Model;

	public:

		ModelGeometry();

		bool						isSkinned() const { return m_desc.isSkinned; }
		
		const VertexFormat*			getVertexFormat() const { return m_pVertexFormat; }

		const IndexBuffer&			getIndexBuffer() const { return m_indexBuffer; }
		const VertexBuffer&			getVertexBuffer() const { return m_vertexBuffer; }

		const uint8*				getVertexData() const { return m_vertexData.getData(); }
		uint						getVertexCount() const { return m_vertexData.getCount(); }

		const uint16*				getIndexData() const { return m_indexData.getData(); }
		uint						getIndexCount() const { return m_indexData.getCount(); }

		//void						getMinMax( Vector3& min, Vector3& max ) const;

		const ModelGeometryDesc&	getDesc() const { return m_desc; }

	private:

		const VertexFormat*		m_pVertexFormat;

		ModelGeometryDesc		m_desc;

		StaticArray< uint8 >	m_vertexData;
		StaticArray< uint16 >	m_indexData;

		IndexBuffer				m_indexBuffer;
		VertexBuffer			m_vertexBuffer;

		bool					initialize( GraphicsSystem& graphicsSystem, const ModelGeometryInitData& initData, const Material* pMaterial );
		void					dispose( GraphicsSystem& graphicsSystem );

	};
}

#endif // TIKI_MODELGEOMETRY_HPP__
