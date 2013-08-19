
#include "tiki/graphics/modelhierarchy.hpp"

namespace tiki
{
	struct ModelHierarchyBinary
	{
		uint16	jointCount;
		uint16	alignedJointCount;
	};

	const void* ModelHierarchy::initialize( const void* pData )
	{
		const ModelHierarchyBinary* pInfo = (const ModelHierarchyBinary*)pData;
		pData = addPtr( pData, sizeof( ModelHierarchyBinary ) );

		m_jointCount		= pInfo->jointCount;
		m_alignedJointCount	= pInfo->alignedJointCount;

		m_jointNames.create( (const crc32*)pData, m_jointCount );
		pData = addPtr( pData, sizeof( crc32 ) * m_jointCount );

		m_parentIndices.create( (const uint16*)pData, m_jointCount );
		pData = addPtr( pData, sizeof( uint16 ) * m_jointCount );

		pData = alignPtr( pData, TIKI_SIMD_ALIGNMENT );
		m_defaultPose.create( (const vf32*)pData, m_alignedJointCount * 3u, TIKI_SIMD_ALIGNMENT );
		pData = addPtr( pData, sizeof( vf32 ) * m_alignedJointCount * 3u );

		pData = alignPtr( pData, TIKI_SIMD_ALIGNMENT );
		m_skinToBoneMatrices.create( (const Matrix*)pData, m_jointCount, TIKI_SIMD_ALIGNMENT );
		pData = addPtr( pData, sizeof( Matrix ) * m_jointCount );

		return pData;
	}

	void ModelHierarchy::dispose()
	{
		m_jointNames.dispose();
		m_parentIndices.dispose();
		m_defaultPose.dispose();
		m_skinToBoneMatrices.dispose();
	}
}