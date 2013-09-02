
#include "tiki/modelexport/toolmodelhierarchy.hpp"

#include "tiki/base/list.hpp"
#include "tiki/toolbase/tikixml.hpp"

namespace tiki
{
	ToolModelHierarchy::ToolModelHierarchy()
	{
		m_pXml				= nullptr;
		m_finalJointCount	= TIKI_SIZE_T_MAX;
		m_hasFinalIndices	= false;
	}

	void ToolModelHierarchy::create( const TikiXml* pXml, const _XmlElement* pNode, float scale )
	{
		TIKI_ASSERT( pXml );
		m_pXml	= pXml;

		List< ToolModelJoint > nodes;
		searchNodes( nodes, pNode );

		m_joints.create( nodes.getData(), nodes.getCount() );

		for (size_t i = 0u; i < m_joints.getCount(); ++i)
		{
			ToolModelJoint& joint = m_joints[ i ];

			const XmlAttribute* pIdAtt = pXml->findAttributeByName( "id", joint.pNode );
			TIKI_ASSERT( pIdAtt );

			joint.name	= pIdAtt->content;
			joint.crc	= crcString( joint.name );

			const XmlElement* pMatrix = pXml->findFirstChild( "matrix", joint.pNode );
			
			if ( pMatrix == nullptr )
			{
				continue;
			}

			parseMatrix( joint.defaultPose, pMatrix->content, scale );
		}
	}

	void ToolModelHierarchy::dispose()
	{
		m_joints.dispose();
	}

	const ToolModelJoint* ToolModelHierarchy::getJointByName( const string& name ) const
	{
		const crc32 crc = crcString( name );

		for (size_t i = 0u; i < m_joints.getCount(); ++i)
		{
			const ToolModelJoint& joint = m_joints[ i ];

			if ( joint.crc == crc )
			{
				return &joint;
			}
		}

		return nullptr;
	}

	void ToolModelHierarchy::markJointAsUsed( const ToolModelJoint& constJoint )
	{
		const size_t index		= m_joints.getIndex( constJoint );
		ToolModelJoint& joint	= m_joints[ index ];
		joint.used = true;
	}

	void ToolModelHierarchy::setBindMatrix( const ToolModelJoint& constJoint, const Matrix& matrix )
	{
		const size_t index		= m_joints.getIndex( constJoint );
		ToolModelJoint& joint	= m_joints[ index ];
		joint.skinToBone = matrix;
	}

	void ToolModelHierarchy::searchNodes( List< ToolModelJoint >& targetList, const XmlElement* pNode )
	{
		const string nodeTag		= "node";
		const XmlElement* pChild	= m_pXml->findFirstChild( nodeTag, pNode );

		size_t parentIndex = targetList.getCount() - 1u;

		while ( pChild )
		{
			ToolModelJoint joint;
			joint.index				= targetList.getCount();
			joint.pNode				= pChild;
			joint.pParentNode		= pNode;
			joint.parentIndex		= parentIndex;
			joint.used				= false;
			joint.finalIndex		= TIKI_SIZE_T_MAX;
			joint.finalParentIndex	= TIKI_SIZE_T_MAX;

			targetList.add( joint );

			searchNodes( targetList, pChild );

			pChild = m_pXml->findNext( nodeTag, pChild );
		}
	}
	
	void ToolModelHierarchy::calculateFinalIndices()
	{
		// mark parents as used
		for (size_t i = 0u; i < m_joints.getCount(); ++i)
		{
			if ( m_joints[ i ].parentIndex != TIKI_SIZE_T_MAX )
			{
				size_t parentIndex = m_joints[ i ].parentIndex;

				while ( parentIndex != TIKI_SIZE_T_MAX )
				{
					m_joints[ parentIndex ].used = true;

					parentIndex = m_joints[ parentIndex ].parentIndex;
				}
			}
		}

		size_t index = 0u;
		Array< size_t > mappingTable;
		mappingTable.create( m_joints.getCount() );

		for (size_t i = 0u; i < m_joints.getCount(); ++i)
		{
			if ( m_joints[ i ].used )
			{
				mappingTable[ i ]				= index;
				m_joints[ i ].finalIndex		= index;
				m_joints[ i ].finalParentIndex	= mappingTable[ m_joints[ i ].parentIndex ];

				index++;
			}
		}

		m_finalJointCount	= index;
		m_hasFinalIndices	= true;

		mappingTable.dispose();
	}
}