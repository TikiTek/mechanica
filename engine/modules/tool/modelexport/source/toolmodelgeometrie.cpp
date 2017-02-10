#include "tiki/modelexport/toolmodelgeometrie.hpp"

#include "tiki/base/basetypes.hpp"
#include "tiki/base/crc32.hpp"
#include "tiki/base/string_tools.hpp"
#include "tiki/math/matrix.hpp"
#include "tiki/modelexport/toolmodelhierarchy.hpp"
#include "tiki/toolbase/tokenizer.hpp"

#include "toolmodelsource.hpp"

namespace tiki
{
	ToolModelSourceBase* setSourceSemantic( Array< ToolModelSourceBase* >& sources, string id, string semantic )
	{
		TIKI_ASSERT( id[ 0u ] == '#' );
		const string sourceId = id.subString( 1u );
		const string semanticName = semantic.toLower();

		string targetField;
		VertexSementic vertexSemantic = VertexSementic_Invalid;

		if ( semanticName == "position")
		{
			targetField		= "position";
			vertexSemantic	= VertexSementic_Position;
		}
		else if ( semanticName == "normal")
		{
			targetField		= "normal";
			vertexSemantic	= VertexSementic_Normal;
		}
		else if ( semanticName == "texcoord" )
		{
			targetField		= "texcoord";
			vertexSemantic	= VertexSementic_TexCoord;
		}
		else if ( semanticName == "joint" )
		{
			targetField		= "jointIndices";
			vertexSemantic	= VertexSementic_JointIndex;
		}
		else if ( semanticName == "weight" )
		{
			targetField		= "jointWeights";
			vertexSemantic	= VertexSementic_JointWeight;
		}
		else if ( semanticName == "vertex" )
		{
			return nullptr;
		}
		else
		{
			TIKI_TRACE_ERROR( "not supported semantic name: %s\n", semanticName.cStr() );
			return nullptr;
		}

		ToolModelSourceBase* pSource = nullptr;
		for (uint i = 0u; i < sources.getCount(); ++i)
		{
			if ( sources[ i ]->id == sourceId )
			{
				pSource = sources[ i ];
				break;
			}
		}
		TIKI_ASSERT( pSource );

		pSource->target		= targetField;
		pSource->semantic	= vertexSemantic;

		return pSource;
	}

	ToolModelSourceBase* getSourceSemantic( Array< ToolModelSourceBase* >& sources, VertexSementic semantic )
	{
		for (uint i = 0u; i < sources.getCount(); ++i)
		{
			if ( sources[ i ]->semantic == semantic )
			{
				return sources[ i ];
			}
		}

		return nullptr;
	}

	void ToolModelGeometrie::create( const XmlReader* pXml, const ToolModelGeometryInstance& instance, const float scaling /*= 1.0f*/ )
	{
		const XmlAttribute* pIdAtt		= pXml->findAttributeByName( "id", instance.pNode );
		const XmlAttribute* pNameAtt	= pXml->findAttributeByName( "name", instance.pNode );

		if ( pIdAtt == nullptr || pNameAtt == nullptr )
		{
			TIKI_TRACE_ERROR( "ToolModelGeometrie: Id or Name attribute not found.\n" );
			return;
		}

		m_vertexFormat.create();

		m_desc.id				= pIdAtt->content;
		m_desc.name				= pNameAtt->content;
		m_desc.scale			= scaling;
		m_desc.instanceMatrix	= instance.worldTransform;

		m_pGeometryNode			= instance.pNode;

		if ( m_desc.id != instance.geometryId )
		{
			TIKI_TRACE_ERROR( "ToolModelGeometrie: instance has a wrong id.\n" );
			return;
		}

		m_pMeshNode		= pXml->findFirstChild( "mesh", instance.pNode );
		if ( m_pMeshNode == nullptr )
		{
			TIKI_TRACE_ERROR( "ToolModelGeometrie: mesh node not found.\n" );
			return;
		}

		// sources
		XmlElementList sourcesNodes;
		Array< ToolModelSource< float > > sources;
		Array< ToolModelSourceBase* > baseSources;
		sources.create( pXml->getChilds( sourcesNodes, m_pMeshNode, "source" ) );

		{
			ToolModelSourceBase* pSources[ 32u ];

			TIKI_ASSERT( sources.getCount() < TIKI_COUNT( pSources ) );
			for (uint i = 0u; i < sources.getCount(); ++i)
			{
				pSources[ i ] = &sources[ i ];
			}

			baseSources.create( pSources, sources.getCount() );
		}

		for (uint i = 0u; i < sources.getCount(); ++i)
		{
			sources[ i ].create( pXml, sourcesNodes[ i ], stringSourceToFloat, 1u );
			sources[ i ].userData = TIKI_SIZE_T_MAX;
		}

		sourcesNodes.dispose();

		// parse vertex layout
		uint inputCount = 0u;
		{
			const XmlElement* pVertices = pXml->findFirstChild( "vertices", m_pMeshNode );

			while ( pVertices != nullptr )
			{
				//const XmlAttribute* pIdAtt = pXml->findAttributeByName( "id", pVertices );
				//TIKI_ASSERT( pIdAtt != nullptr );

				const XmlElement* pInput = pXml->findFirstChild( "input", pVertices );

				while ( pInput != nullptr )
				{
					const XmlAttribute* pSourceAtt = pXml->findAttributeByName( "source", pInput );
					TIKI_ASSERT( pSourceAtt );

					const XmlAttribute* pSemanticAtt = pXml->findAttributeByName( "semantic", pInput );
					TIKI_ASSERT( pSemanticAtt );

					ToolModelSourceBase* pSource = setSourceSemantic( baseSources, pSourceAtt->content, pSemanticAtt->content );
					pSource->userData = 0u;

					m_vertexFormat.addSemantic( pSource->semantic );

					pInput = pXml->findNext( "input", pInput );
				}

				inputCount++;

				pVertices = pXml->findNext( "vertices", pVertices );
			}
		}

		// parse triangles
		{
			bool polymode = false;
			const XmlElement* pTriangles = pXml->findFirstChild( "triangles", m_pMeshNode );
			if ( pTriangles == nullptr )
			{
				pTriangles = pXml->findFirstChild( "polylist", m_pMeshNode );
				polymode = true;

				if ( pTriangles == nullptr )
				{
					TIKI_TRACE_ERROR( "no triangles/polylist node in geometry '%s'\n", m_desc.name.cStr() );
					return;
				}
			}

			if (polymode)
			{
				const XmlElement* pVertexCount = pXml->findFirstChild( "vcount", pTriangles );

				Tokenizer token;
				token.create( pVertexCount->content, " " );

				uint index = 0u;
				while ( index != -1 )
				{
					const string value = token.findNext( &index ).trim();

					if ( !value.isEmpty() )
					{
						uint i = string_tools::parseUInt32( value.cStr() );

						if ( i != 3u )
						{
							TIKI_TRACE_ERROR( "model ist not triangulated.\n" );
							return;
						}
					}
				}
			}

			//const XmlAttribute* pCountAtt = pXml->findAttributeByName( "count", pTriangles );
			//if ( pCountAtt == nullptr )
			//{
			//	TIKI_TRACE_ERROR( "triangle node has no count attribute" );
			//	return;
			//}

			//uint vertexCount = ParseString::parseUInt32( pCountAtt->content );

			const XmlElement* pInput = pXml->findFirstChild( "input", pTriangles );
			while ( pInput )
			{
				const XmlAttribute* pSourceAtt		= pXml->findAttributeByName( "source", pInput );
				const XmlAttribute* pSemanticAtt	= pXml->findAttributeByName( "semantic", pInput );
				TIKI_ASSERT( pSourceAtt );
				TIKI_ASSERT( pSemanticAtt );

				ToolModelSourceBase* pSource = setSourceSemantic( baseSources, pSourceAtt->content, pSemanticAtt->content );

				if ( pSource != nullptr )
				{
					pSource->userData = inputCount;
					m_vertexFormat.addSemantic( pSource->semantic );

					inputCount++;
				}

				pInput = pXml->findNext( "input", pInput );
			}

			// read indices
			List< uint > indicesData;
			{
				List< uint > indices;
				const XmlElement* pNode = pXml->findFirstChild( "p", pTriangles );

				Tokenizer token;
				token.create( pNode->content, " " );

				uint index = 0u;
				while ( index != -1 )
				{
					string value = token.findNext( &index ).trim();

					if ( !value.isEmpty() )
					{
						uint i = string_tools::parseUInt32( value.cStr() );

						indices.add( i );
					}
				}

				const ToolModelSourceBase* pPosSource = getSourceSemantic( baseSources, VertexSementic_Position );
				TIKI_ASSERT( pPosSource != nullptr );

				List< crc32 > indicesCrc;
				List< uint > indicesCrcIndex;
				List< crc32 > indicesSkinningCrc;
				for (uint i = 0u; i < indices.getCount(); i += inputCount)
				{
					const crc32 indexCrc = crcBytes( &indices[ i ], 3u * sizeof( indices[ i ] ) );
					indicesCrc.add( indexCrc );

					const crc32 skinningCrc = crcBytes( &indices[ i + pPosSource->userData ], sizeof( indices[ 0u ] ) );
					indicesSkinningCrc.add( skinningCrc );

					if ( !indicesCrcIndex.contains( indexCrc ) )
					{
						indicesCrcIndex.add( indexCrc );

						for (uint j = 0u; j < inputCount; ++j)
						{
							indicesData.add( indices[ i + j ] );
						}
					}

					const sint crcIndex = indicesCrcIndex.indexOf( indexCrc );
					TIKI_ASSERT( crcIndex >= 0 );
					m_indices.add( uint32( crcIndex ) );
				}

				List< uint > indicesSkinningData;
				List< uint > indicesSkinningCount;
				List< uint > indicesSkinningOffset;
				for (uint i = 0u; i < indicesSkinningCrc.getCount(); ++i)
				{
					const crc32 crc = indicesSkinningCrc[ i ];

					if ( crc == 0u )
					{
						continue;
					}

					uint count = 0u;

					indicesSkinningOffset.add( indicesSkinningData.getCount() );

					for (uint j = i; j < indicesSkinningCrc.getCount(); ++j )
					{
						if ( crc == indicesSkinningCrc[ j ] )
						{
							count++;
							indicesSkinningData.add( j );
							indicesSkinningCrc[ j ] = 0u;
						}
					}

					indicesSkinningCount.add( count );
				}

				for (uint i = 0u; i < m_indices.getCount(); i += 3u)
				{
					const uint32 index0 = m_indices[ i + 0u ];
					const uint32 index1 = m_indices[ i + 1u ];
					const uint32 index2 = m_indices[ i + 2u ];

					m_indices[ i + 0u ] = index0;
					m_indices[ i + 1u ] = index1;
					m_indices[ i + 2u ] = index2;
				}

				m_vertices.resize( indicesCrcIndex.getCount() );

				m_skinningIndicesData.addRange( indicesSkinningData.getBegin(), indicesSkinningData.getCount() );
				m_skinningIndicesCount.addRange( indicesSkinningCount.getBegin(), indicesSkinningCount.getCount() );
				m_skinningIndicesOffset.addRange( indicesSkinningOffset.getBegin(), indicesSkinningOffset.getCount() );
			}

			// fill vertices
			uint fullStride = 0u;
			for (uint j= 0u; j < sources.getCount(); ++j)
			{
				fullStride += sources[ j ].stride;
			}

			for (uint i = 0u; i < sources.getCount(); ++i)
			{
				const ToolModelSource< float >& source = sources[ i ];

				if ( source.userData == TIKI_SIZE_T_MAX )
				{
					continue;
				}

				if ( source.target.isEmpty() )
				{
					TIKI_TRACE_WARNING( "source with id '%s' has no target and will be ignored.", source.id.cStr() );
					continue;
				}

				uint indexOffset = 0u;
				for (uint j= 0u; j < i; ++j)
				{
					indexOffset += sources[ i ].stride;
				}

				for (uint j = 0u; j < source.stride; ++j)
				{
					uint offset = 0u;
					if ( source.target == "position" )
					{
						offset = TIKI_OFFSETOF( ToolModelVertex, position );
					}
					else if ( source.target == "normal" )
					{
						offset = TIKI_OFFSETOF( ToolModelVertex, normal );
					}
					else if ( source.target == "tangent" )
					{
						offset = TIKI_OFFSETOF( ToolModelVertex, tangent );
					}
					else if ( source.target == "binormal" )
					{
						offset = TIKI_OFFSETOF( ToolModelVertex, binormal );
					}
					else if ( source.target == "texcoord" )
					{
						offset = TIKI_OFFSETOF( ToolModelVertex, texcoord );
					}
					else if ( source.target == "color" )
					{
						offset = TIKI_OFFSETOF( ToolModelVertex, color );
					}
					else if ( source.target == "jointIndices" )
					{
						offset = TIKI_OFFSETOF( ToolModelVertex, jointIndices );
					}
					else if ( source.target == "jointWeights" )
					{
						offset = TIKI_OFFSETOF( ToolModelVertex, jointWeights );
					}

					if ( source.techniques[ j ].name == "y" )
					{
						offset += 4u;
					}
					else if ( source.techniques[ j ].name == "z" )
					{
						offset += 8u;
					}
					else if ( source.techniques[ j ].name == "w" )
					{
						offset += 12u;
					}
					TIKI_ASSERT( offset < sizeof( ToolModelVertex ) );

					float scale = ( source.semantic == VertexSementic_Position ? scaling : 1.0f );

					for (uint k = 0u; k < m_vertices.getCount(); ++k)
					{
						ToolModelVertex& vertex = m_vertices[ k ];
						float* pTarget = addPointerCast< float >( &vertex, offset );

						const uint dataIndex1 = (k * inputCount) + source.userData;
						TIKI_ASSERT( dataIndex1 != indicesData.getCount() );
						const uint dataIndex2 = (indicesData[ dataIndex1 ] * source.stride) + j;

						const float value = source.data[ dataIndex2 ] * scale;
						*pTarget = value;
					}
				}
			}
		}

		for (uint i = 0u; i < sources.getCount(); ++i)
		{
			sources[ i ].dispose();
		}
		sources.dispose();
		baseSources.dispose();
	}

	void ToolModelGeometrie::applySkinning( ToolModelHierarchy& hierarchy, const XmlReader* pXml, const XmlElement* pSkinNode )
	{
		TIKI_ASSERT( pXml );
		TIKI_ASSERT( pSkinNode );

		m_desc.isSkinned = true;

		ToolModelSource< string > boneNames;
		ToolModelSource< Matrix44 > matrices;
		ToolModelSource< float > weights;
		Array< ToolModelSourceBase* > baseSources;

		{
			ToolModelSourceBase* pSources[] = { &boneNames, &matrices, &weights };
			baseSources.create( pSources, TIKI_COUNT( pSources ) );
		}

		// parse shape matrix
		{
			const XmlElement* pMatrix = pXml->findFirstChild( "bind_shape_matrix", pSkinNode );

			if ( pMatrix != nullptr )
			{
				parseMatrix( m_desc.shapeMatrix, pMatrix->content, m_desc.scale );
			}
		}

		// parse sources
		const XmlElement* pSourceNode = pXml->findFirstChild( "source", pSkinNode );
		while ( pSourceNode )
		{
			const XmlAttribute* pIdAtt = pXml->findAttributeByName( "id", pSourceNode );
			TIKI_ASSERT( pIdAtt );

			string id = string( pIdAtt->content ).toLower();

			if ( id.endsWith( "joints" ) )
			{
				boneNames.create( pXml, pSourceNode, stringSourceToString, 1u );
			}
			else if ( id.endsWith( "matrices" ) )
			{
				matrices.create( pXml, pSourceNode, stringSourceToMatrix, 16u );
			}
			else if ( id.endsWith( "weights" ) )
			{
				weights.create( pXml, pSourceNode, stringSourceToFloat, 1u );
			}

			pSourceNode = pXml->findNext( "source", pSourceNode );
		}

		// calculate joint ids
		Array< uint32 > jointIndices;
		jointIndices.create( boneNames.data.getCount() );

		for (uint i = 0u; i < boneNames.data.getCount(); ++i)
		{
			const string& name				= boneNames.data[ i ];
			const ToolModelJoint* pJoint	= hierarchy.getJointByName( name );

			if ( !pJoint )
			{
				TIKI_TRACE_ERROR( "Joint with name '%s' not found", name.cStr() );
				return;
			}

			jointIndices[ i ] = pJoint->index;

			Matrix44 mtx = matrices.data[ i ];

			// scale translation
			mtx.w.x *= m_desc.scale;
			mtx.w.y *= m_desc.scale;
			mtx.w.z *= m_desc.scale;

			hierarchy.markJointAsUsed( *pJoint );
			hierarchy.setBindMatrix( *pJoint, mtx );
		}

		// parse technique
		const XmlElement* pVertexWeights = pXml->findFirstChild( "vertex_weights", pSkinNode );
		TIKI_ASSERT( pVertexWeights );

		uint inputCount = 0u;
		{
			const XmlElement* pInput = pXml->findFirstChild( "input", pVertexWeights );

			while ( pInput != nullptr )
			{
				const XmlAttribute* pAttSource = pXml->findAttributeByName( "source", pInput );
				const XmlAttribute* pAttSemantic = pXml->findAttributeByName( "semantic", pInput );

				ToolModelSourceBase* pSource = setSourceSemantic( baseSources, pAttSource->content, pAttSemantic->content );
				pSource->userData = inputCount++;

				pInput = pXml->findNext( pInput );
			}
		}

		//prepare vertices
		for (uint i = 0u; i < m_vertices.getCount(); ++i)
		{
			ToolModelVertex& vertex = m_vertices[ i ];

			memory::zero( &vertex.jointIndices.x, sizeof( vertex.jointIndices ) );
			memory::zero( &vertex.jointWeights.x, sizeof( vertex.jointWeights ) );

			vertex.jointWeights.x = 1.0f;
		}

		// parse vertices
		{
			const XmlElement* pV = pXml->findFirstChild( "v", pVertexWeights );
			const XmlElement* pCount = pXml->findFirstChild( "vcount", pVertexWeights );
			TIKI_ASSERT( pV );
			TIKI_ASSERT( pCount );

			Tokenizer token;
			token.create( pV->content, " \t\n\r", true );

			Tokenizer tokenCount;
			tokenCount.create( pCount->content, " \t\n\r", true );

			uint i = 0u;
			uint c = 0u;
			uint count = TIKI_SIZE_T_MAX;
			uint index = 0u;
			uint sourceIndex = 0u;
			while ( index != TIKI_SIZE_T_MAX )
			{
				if ( c == count || count == TIKI_SIZE_T_MAX )
				{
					//TIKI_ASSERT( f32::isEquals( m_vertices[ i ].jointWeights.x + m_vertices[ i ].jointWeights.y + m_vertices[ i ].jointWeights.z + m_vertices[ i ].jointWeights.w, 1.0f, 0.01f ) );

					if ( count != TIKI_SIZE_T_MAX ) i++;

					const string countString	= tokenCount.findNext();
					count						= string_tools::parseUInt32( countString.cStr() );
					c = 0u;

					TIKI_ASSERT( count < 5u );
				}

				const string part	= token.findNext( &index );
				const uint value	= string_tools::parseUInt32( part.cStr() );

				for (uint j = 0u; j < m_skinningIndicesCount[ i ]; ++j)
				{
					const uint vertexIndex = m_skinningIndicesOffset[ i ] + j;
					ToolModelVertex& vertex = m_vertices[ m_skinningIndicesData[ vertexIndex ] ];

					Vector3 pos;
					vector::set( pos, vertex.position );
					matrix::transform( pos, m_desc.shapeMatrix );
					vertex.position.x = pos.x;
					vertex.position.y = pos.y;
					vertex.position.z = pos.z;

					if ( sourceIndex == 0u )
					{
						uint32* pIndices = &vertex.jointIndices.x;

						const uint32 jointIndex = jointIndices[ value ];
						TIKI_ASSERT( jointIndex < hierarchy.getJointCount() );

						pIndices[ c ] = jointIndex;
					}
					else if ( sourceIndex == 1u )
					{
						float* pWeights = &vertex.jointWeights.x;

						const float weight = weights.data[ value ];
						TIKI_ASSERT( weight >= 0.0f && weight <= 1.0f );

						pWeights[ c ] = weight;
					}
				}

				if ( ++sourceIndex == inputCount )
				{
					c++;
					sourceIndex = 0u;
				}
			}
		}

		m_vertexFormat.addSemantic( VertexSementic_JointIndex );
		m_vertexFormat.addSemantic( VertexSementic_JointWeight );

		jointIndices.dispose();
		baseSources.dispose();

		boneNames.dispose();
		matrices.dispose();
		weights.dispose();
	}

	void ToolModelGeometrie::dispose()
	{
		m_indices.dispose();
		m_vertices.dispose();
		m_skinningIndicesData.dispose();
		m_skinningIndicesCount.dispose();
		m_skinningIndicesOffset.dispose();

		m_vertexFormat.dispose();
	}

	bool ToolModelGeometrie::calculateTangents()
	{
		if ( !m_vertexFormat.hasSemantic( VertexSementic_Normal ) )
		{
			return false;
		}

		Array< Vector3 > tan1;
		Array< Vector3 > tan2;
		List< uint > indices;
		tan1.create( m_vertices.getCount() );
		tan2.create( m_vertices.getCount() );

		for (uint i = 0u; i < m_indices.getCount(); i += 3u)
		{
			uint i1 = m_indices[ i + 0u ];
			uint i2 = m_indices[ i + 1u ];
			uint i3 = m_indices[ i + 2u ];

			Vector3 v1;
			Vector3 v2;
			Vector3 v3;
			vector::set( v1, m_vertices[ i1 ].position );
			vector::set( v2, m_vertices[ i2 ].position );
			vector::set( v3, m_vertices[ i3 ].position );

			Vector2 w1;
			Vector2 w2;
			Vector2 w3;
			vector::set( w1, m_vertices[ i1 ].texcoord );
			vector::set( w2, m_vertices[ i2 ].texcoord );
			vector::set( w3, m_vertices[ i3 ].texcoord );

			const float x1 = v2.x - v1.x;
			const float x2 = v3.x - v1.x;
			const float y1 = v2.y - v1.y;
			const float y2 = v3.y - v1.y;
			const float z1 = v2.z - v1.z;
			const float z2 = v3.z - v1.z;

			const float s1 = w2.x - w1.x;
			const float s2 = w3.x - w1.x;
			const float t1 = w2.y - w1.y;
			const float t2 = w3.y - w1.y;

			const float r = 1.0f / (s1 * t2 - s2 * t1);
			const Vector3 sdir = { (t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r };
			const Vector3 tdir = { (s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r };

			if ( !indices.contains( i1 ) )
			{
				vector::add( tan1[ i1 ], sdir );
				vector::add( tan2[ i1 ], tdir );
				indices.add( i1 );
			}

			if ( !indices.contains( i2 ) )
			{
				vector::add( tan1[ i2 ], sdir );
				vector::add( tan2[ i2 ], tdir );
				indices.add( i2 );
			}

			if ( !indices.contains( i3 ) )
			{
				vector::add( tan1[ i3 ], sdir );
				vector::add( tan2[ i3 ], tdir );
				indices.add( i3 );
			}
		}

		for (uint i = 0u; i < m_vertices.getCount(); ++i)
		{
			const Vector3& t = tan1[ i ];

			Vector3 n;
			vector::set( n, m_vertices[ i ].normal );

			// Gram-Schmidt orthogonalize
			Vector3 mul = n;
			vector::scale( mul, vector::dot( n, t ) );

			Vector3 sub = t;
			vector::normalizeZero( vector::sub( sub, mul ) );

			createFloat4( m_vertices[ i ].tangent, sub.x, sub.y, sub.z, 0.0f );

			// Calculate handedness
			Vector3 cross;
			vector::cross( cross, n, t );

			m_vertices[ i ].tangent.w = ( vector::dot( cross, tan2[ i ] ) < 0.0f ) ? -1.0f : 1.0f;
		}

		tan1.dispose();
		tan2.dispose();

		m_vertexFormat.insertSemantic( VertexSementic_TangentFlip, VertexSementic_Normal );

		return true;
	}

	void ToolModelGeometrie::transformToInstance()
	{
		Matrix44 worldTransform = m_desc.instanceMatrix;
		matrix::transpose( worldTransform );

		if ( matrix::isIdentity( worldTransform ) )
		{
			return;
		}

		const Vector3 rotX = { worldTransform.x.x, worldTransform.x.y, worldTransform.x.z };
		const Vector3 rotY = { worldTransform.y.x, worldTransform.y.y, worldTransform.y.z };
		const Vector3 rotZ = { worldTransform.z.x, worldTransform.z.y, worldTransform.z.z };

		Matrix33 normalMatrix;
		matrix::set( normalMatrix, rotX, rotY, rotZ );

		for (uint i = 0u; i < m_vertices.getCount(); ++i)
		{
			ToolModelVertex& vertex = m_vertices[ i ];

			Vector3 position	= { vertex.position.x, vertex.position.y, vertex.position.z };
			Vector3 normal		= { vertex.normal.x, vertex.normal.y, vertex.normal.z };
			matrix::transform( position, worldTransform );
			matrix::transform( normal, normalMatrix );
			createFloat3( vertex.position, position.x, position.y, position.z );
			createFloat3( vertex.normal, normal.x, normal.y, normal.z );
		}
	}
}