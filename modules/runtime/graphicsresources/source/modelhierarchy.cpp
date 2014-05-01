
#include "tiki/graphics/modelhierarchy.hpp"

#include "tiki/resource/resourcefile.hpp"

namespace tiki
{
	struct ModelHierarchyInitData
	{
		uint16				jointCount;
		uint16				alignedJointCount;

		ResRef< crc32 >		jointNames;
		ResRef< uint16 >	parentJoints;
		ResRef< vf32 >		defaultPose;
		ResRef< Matrix44 >	skinToJointMatrices;
	};

	bool ModelHierarchy::initialize( const ModelHierarchyInitData& initData )
	{
		m_jointCount		= initData.jointCount;
		m_alignedJointCount	= initData.alignedJointCount;

		m_jointNames.create( initData.jointNames.getData(), m_jointCount );
		m_parentIndices.create( initData.parentJoints.getData(), m_jointCount );
		m_defaultPose.create( initData.defaultPose.getData(), m_alignedJointCount * 3u );
		m_skinToBoneMatrices.create( initData.skinToJointMatrices.getData(), m_jointCount );

		return true;
	}

	void ModelHierarchy::dispose()
	{
		m_jointNames.dispose();
		m_parentIndices.dispose();
		m_defaultPose.dispose();
		m_skinToBoneMatrices.dispose();
	}
}