
#include "tiki/modelexport/toolmodelanimation.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/math/matrix.hpp"
#include "tiki/modelexport/toolmodelhierarchy.hpp"
#include "tiki/toolbase/tikixml.hpp"

#include "toolmodelsource.hpp"

namespace tiki
{
	void ToolModelAnimation::create( ToolModelHierarchy& hierarchy, const TikiXml* pXml, const _XmlElement* pNode )
	{
		m_isCreated		= true;
		m_frameCount	= TIKI_SIZE_T_MAX;
		m_joints.create( hierarchy.getJointCount() );

		for (size_t i = 0u; i < m_joints.getCount(); ++i)
		{
			m_joints[ i ].pJoint = &hierarchy.getJointByIndex( i );
		}

		const XmlElement* pAnimation = pXml->findFirstChild( "animation", pNode );

		while ( pAnimation )
		{
			const size_t index = createJoint( pXml, pAnimation, hierarchy );

			if ( index != TIKI_SIZE_T_MAX )
			{
				hierarchy.markJointAsUsed( *m_joints[ index ].pJoint );
			}

			pAnimation = pXml->findNext( "animation", pAnimation );
		}
	}

	void ToolModelAnimation::dispose()
	{
		for (size_t i = 0u; i < m_joints.getCount(); ++i)
		{
			m_joints[ i ].keys.dispose();
			m_joints[ i ].samples.dispose();
		}

		if ( m_joints.getCount() != 0u )
		{
			m_joints.dispose();
		}
	}

	size_t ToolModelAnimation::createJoint( const TikiXml* pXml, const _XmlElement* pParentNode, const ToolModelHierarchy& hierarchy )
	{
		const XmlElement* pNode = pXml->findFirstChild( "animation", pParentNode );

		if ( pNode == nullptr )
		{
			return TIKI_SIZE_T_MAX;
		}

		ToolModelSource< float > times;
		ToolModelSource< Matrix > transforms;
		ToolModelSource< string > interpolators;

		const XmlElement* pSampler = pXml->findFirstChild( "sampler", pNode );
		const XmlElement* pChannel = pXml->findFirstChild( "channel", pNode );

		if ( pSampler == nullptr || pChannel == nullptr )
		{
			return TIKI_SIZE_T_MAX;
		}

		// parse channel
		string jointName;
		{
			const XmlAttribute* pTarget = pXml->findAttributeByName( "target", pChannel );

			const string target = pTarget->content;
			const size_t sli = target.indexOf( '/' );

			const string first = target.substring( 0u, sli );
			const string second = target.substring( sli + 1u );

			if ( second != "matrix" )
			{
				return TIKI_SIZE_T_MAX;
			}

			jointName = first;
		}

		// parse sampler ans sources
		{
			List< const XmlElement* > sourcesNodes;
			List< string > sourcesIds;
			{
				const XmlElement* pSource = pXml->findFirstChild( "source", pNode );

				while ( pSource )
				{
					const XmlAttribute* pIdAtt = pXml->findAttributeByName( "id", pSource );
					TIKI_ASSERT( pIdAtt );

					sourcesIds.add( pIdAtt->content );
					sourcesNodes.add( pSource );

					pSource = pXml->findNext( "source", pSource );
				}

			}

			const XmlElement* pInput = pXml->findFirstChild( "input", pSampler );

			while ( pInput )
			{
				const XmlAttribute* pSemanicAtt	= pXml->findAttributeByName( "semantic", pInput );
				const XmlAttribute* pSourceAtt	= pXml->findAttributeByName( "source", pInput );

				const string semanticName	= string( pSemanicAtt->content ).toLower();
				const string sourceName		= string( pSourceAtt->content ).substring( 1u );

				const XmlElement* pSource = nullptr;

				for (size_t i = 0u; i < sourcesIds.getCount(); ++i)
				{
					if ( sourcesIds[ i ] == sourceName )
					{
						pSource = sourcesNodes[ i ];
						break;
					}
				}

				if ( pSource == nullptr )
				{
					continue;
				}

				if ( semanticName == "input" )
				{
					times.create( pXml, pSource, stringSourceToFloat, 1u );
				}
				else if ( semanticName == "output" )
				{
					transforms.create( pXml, pSource, stringSourceToMatrix, 16u );
				}
				else if ( semanticName == "interpolation" )
				{
					interpolators.create( pXml, pSource, stringSourceToString, 1u );
				}

				pInput = pXml->findNext( "input", pInput );
			}
		}

		size_t index = TIKI_SIZE_T_MAX;

		if ( times.isCreated() && transforms.isCreated() ) // && interpolators.isCreated()
		{
			const ToolModelJoint* pHierachyJoint = hierarchy.getJointByName( jointName );

			if ( pHierachyJoint != nullptr )
			{
				index = hierarchy.getJointData().getIndex( *pHierachyJoint );
				ToolModelAnimationJoint& joint = m_joints[ index ];

				size_t count = times.data.getCount();
				if ( times.data.getCount() != ( transforms.data.getCount() / 16u ) )
				{
					TIKI_TRACE_WARNING( "different times and transforms count in joint '%s'.\n", pHierachyJoint->name.cStr() );
					count = TIKI_MIN( times.data.getCount(), transforms.data.getCount() );
				}

				if ( count != 0u )
				{
					joint.keys.create( count );

					for (size_t i = 0u; i < count; ++i)
					{
						joint.keys[ i ].time		= times.data[ i ];
						joint.keys[ i ].transform	= transforms.data[ i ];
					}

					const size_t frameCount = (size_t)( joint.keys.getLast()->time * 60.0f );
					m_frameCount = TIKI_MIN( m_frameCount, frameCount );

					joint.samples.create( frameCount );

					const ToolModelKey* pLeftKey = &joint.keys[ 0u ];
					const ToolModelKey* pRightKey = &joint.keys[ 1u ];
					size_t rightIndex = 1u;

					for (size_t i = 0u; i < frameCount; ++i)
					{
						const float time	= (float)i / 60.0f;

						if ( time >= pRightKey->time )
						{
							pLeftKey = pRightKey;

							rightIndex++;
							pRightKey = &joint.keys[ rightIndex ];
						}

						const float koeff = (time - pLeftKey->time) / (pRightKey->time - pLeftKey->time);

						Matrix mtx;
						mtx.lerp( pLeftKey->transform, pRightKey->transform, koeff );

						joint.samples[ i ] = mtx;
					}
				}
			}
		}

		times.dispose();
		transforms.dispose();
		interpolators.dispose();

		return index;
	}

}