#ifndef TIKI_ANIMATION_HPP
#define TIKI_ANIMATION_HPP

#include "tiki/base/types.hpp"
#include "tiki/resource/resource.hpp"
#include "tiki/resource/resourcefile.hpp"

namespace tiki
{
	struct AnimationJoint;
	struct ModelHierarchy;

	struct AnimationChunkHeader
	{
		uint32	dataOffset;

		uint16	startTime;
		uint16	endTime;

		uint16	usedRotationJointCount;
		uint16	interpolatedRotationJointCount;
		uint16	defaultPoseRotationJointCount;

		uint16	usedPositionJointCount;
		uint16	interpolatedPositionJointCount;
		uint16	defaultPosePositionJointCount;

		uint16	usedScaleJointCount;
		uint16	interpolatedScaleJointCount;
		uint16	defaultPoseScaleJointCount;
	};

	struct AnimationData
	{
		float			positionFactor;
		float			positionTangentFactor;
		float			scaleFactor;
		float			scaleTangentFactor;

		uint16			headerOffset;
		uint16			headerCount;

		uint16			frameCount;
		uint16			jointCount;

		ResRef< uint8 >	data;
	};

	class Animation : public Resource
	{
		TIKI_DEFINE_RESOURCE( Animation, TIKI_FOURCC( 'A', 'N', 'I', 'M' ) );

	public:

		static void				registerResourceType( ResourceManager& resourceManager );
		static void				unregisterResourceType( ResourceManager& resourceManager );

		void					sample( AnimationJoint* pTargetJoints, size_t jointCount, const ModelHierarchy& hierarchy, const float time ) const;

	protected:

		virtual bool			createInternal( const ResourceInitData& initData, const FactoryContext& factoryContext );
		virtual void			disposeInternal( const FactoryContext& factoryContext );

	private:

								Animation();
		virtual					~Animation();

		const AnimationData*	m_pData;

	};
}

#endif // TIKI_ANIMATION_HPP
