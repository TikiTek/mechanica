#ifndef TIKI_TOOLMODELGEOMETRIE_HPP__
#define TIKI_TOOLMODELGEOMETRIE_HPP__

#include "tiki/base/dynamic_string.hpp"
#include "tiki/container/array.hpp"
#include "tiki/io/xmlreader.hpp"
#include "tiki/math/matrix.hpp"
#include "tiki/math/quaternion.hpp"
#include "tiki/math/vector.hpp"
#include "tiki/modelexport/toolmodelvertexformat.hpp"

#include "base.hpp"

namespace tiki
{
	class ToolModelHierarchy;
	struct ToolModelGeometryInstance;

	struct ToolModelVertex
	{
		float3	position;

		float3	normal;
		float4	tangent;
		float3	binormal;

		float2	texcoord;
		float3	color;

		uint4	jointIndices;
		float4	jointWeights;
	};

	struct ToolModelGeometrieDesc
	{
		ToolModelGeometrieDesc()
		{
			scale			= 1.0f;

			hasNormals		= false;
			hasTangents		= false;
			hasBinormals	= false;

			hasColor		= false;
			hasTexcoord		= false;

			isSkinned		= false;
		}
		
		string		id;
		string		name;

		float		scale;

		bool		hasNormals;
		bool		hasTangents;
		bool		hasBinormals;

		bool		hasColor;
		bool		hasTexcoord;

		bool		isSkinned;
		Matrix44	shapeMatrix;
		Matrix44	instanceMatrix;
	};

	class ToolModelGeometrie
	{
	public:

		void							create( const XmlReader* pXml, const ToolModelGeometryInstance& instance, const float scaling = 1.0f );
		void							dispose();

		bool							calculateTangents();
		void							applySkinning( ToolModelHierarchy& hierarchy, const XmlReader* pXml, const XmlElement* pSkinNode );
		void							transformToInstance();

		uint							getIndexCount() const { return m_indices.getCount(); };
		uint							getIndexByIndex( uint index ) const { return m_indices[ index ]; }
		const uint32*					getIndexData() const { return m_indices.getBegin(); }

		uint							getVertexCount() const { return m_vertices.getCount(); };
		const ToolModelVertex&			getVertexByIndex( uint index ) const { return m_vertices[ index ]; }

		const ToolModelGeometrieDesc&	getDesc() const { return m_desc; }
		const ToolModelVertexFormat&	getVertexFormat() const { return m_vertexFormat; }

	private:

		const XmlElement*				m_pGeometryNode;
		const XmlElement*				m_pMeshNode;

		List< uint32 >					m_indices;
		List< ToolModelVertex >			m_vertices;
		List< uint >					m_skinningIndicesCount;
		List< uint >					m_skinningIndicesOffset;
		List< uint >					m_skinningIndicesData;

		ToolModelGeometrieDesc			m_desc;
		ToolModelVertexFormat			m_vertexFormat;

	};
}

#endif // TIKI_TOOLMODELGEOMETRIE_HPP__
