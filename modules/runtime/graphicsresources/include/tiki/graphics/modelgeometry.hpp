#ifndef TIKI_MODELGEOMETRY_HPP__
#define TIKI_MODELGEOMETRY_HPP__

#include "tiki/base/staticarray.hpp"
#include "tiki/graphics/indexbuffer.hpp"
#include "tiki/graphics/vertexbuffer.hpp"
#include "tiki/graphics/vertexattribute.hpp"
#include "tiki/math/matrix.hpp"
#include "tiki/resource/resourcefile.hpp"

namespace tiki
{
	class GraphicsContext;
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
		ResRef< uint8 >				indexData;
	};

	class ModelGeometry
	{
		TIKI_NONCOPYABLE_CLASS( ModelGeometry );
		friend class Model;

	public:

		ModelGeometry();

		bool						isSkinned() const		{ return m_desc.isSkinned; }
		const ModelGeometryDesc&	getDescription() const	{ return m_desc; }
		//void						getBoundingBox( Vector3& min, Vector3& max ) const;

		const VertexFormat*			getVertexFormat() const	{ return m_pVertexFormat; }

		const uint8*				getVertexData() const	{ return m_vertexData.getBegin(); }
		uint						getVertexCount() const	{ return m_vertexData.getCount(); }

		const uint8*				getIndexData() const	{ return m_indexData.getBegin(); }
		uint						getIndexCount() const	{ return m_desc.indexCount; }

		void						render( GraphicsContext& graphicsContext ) const;

	private:

		const VertexFormat*			m_pVertexFormat;

		ModelGeometryDesc			m_desc;

		StaticArray< const uint8 >	m_vertexData;
		StaticArray< const uint8 >	m_indexData;

		IndexBuffer					m_indexBuffer;
		VertexBuffer				m_vertexBuffer;

		bool						initialize( GraphicsSystem& graphicsSystem, const ModelGeometryInitData& initData, const Material* pMaterial );
		void						dispose( GraphicsSystem& graphicsSystem );

	};
}

#endif // TIKI_MODELGEOMETRY_HPP__
