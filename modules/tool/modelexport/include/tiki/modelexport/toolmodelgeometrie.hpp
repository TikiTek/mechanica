#ifndef TIKI_TOOLMODELGEOMETRIE_HPP__
#define TIKI_TOOLMODELGEOMETRIE_HPP__

#include "tiki/base/array.hpp"
#include "tiki/base/reflection.hpp"
#include "tiki/base/structs.hpp"
#include "tiki/math/matrix.hpp"
#include "tiki/math/quaternion.hpp"
#include "tiki/math/vector.hpp"
#include "tiki/toolbase/tikixml.hpp"
#include "tiki/modelexport/toolmodelvertexformat.hpp"

namespace tiki
{
	class ToolModelHierarchy;
	struct ToolModelGeometryInstance;

	TIKI_REFLECTION_STRUCT(
		ToolModelVertex,
		TIKI_REFLECTION_FIELD( float3, position )

		TIKI_REFLECTION_FIELD( float3, normal )
		TIKI_REFLECTION_FIELD( float4, tangent )
		TIKI_REFLECTION_FIELD( float3, binormal )

		TIKI_REFLECTION_FIELD( float2, texcoord )
		TIKI_REFLECTION_FIELD( float3, color )

		TIKI_REFLECTION_FIELD( uint4, jointIndices )
		TIKI_REFLECTION_FIELD( float4, jointWeights )
	);

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

		void							create( const TikiXml* pXml, const ToolModelGeometryInstance& instance, const float scaling = 1.0f );
		void							dispose();

		bool							calculateTangents();
		void							applySkinning( ToolModelHierarchy& hierarchy, const TikiXml* pXml, const XmlElement* pSkinNode );
		void							transformToInstance();

		uint							getIndexCount() const { return m_indices.getCount(); };
		uint							getIndexByIndex( uint index ) const { return m_indices[ index ]; }
		const uint32*					getIndexData() const { return m_indices.getData(); }

		uint							getVertexCount() const { return m_vertices.getCount(); };
		const ToolModelVertex&			getVertexByIndex( uint index ) const { return m_vertices[ index ]; }

		const ToolModelGeometrieDesc&	getDesc() const { return m_desc; }
		const ToolModelVertexFormat&	getVertexFormat() const { return m_vertexFormat; }

	private:

		const XmlElement*				m_pGeometryNode;
		const XmlElement*				m_pMeshNode;

		Array< uint32 >					m_indices;
		Array< ToolModelVertex >		m_vertices;
		Array< uint >					m_skinningIndicesCount;
		Array< uint >					m_skinningIndicesOffset;
		Array< uint >					m_skinningIndicesData;

		ToolModelGeometrieDesc			m_desc;
		ToolModelVertexFormat			m_vertexFormat;

	};
}

#endif // TIKI_TOOLMODELGEOMETRIE_HPP__
