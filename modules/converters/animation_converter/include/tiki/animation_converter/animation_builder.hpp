#pragma once

#include "tiki/animation_converter/animation_spline.hpp"
#include "tiki/base/types.hpp"
#include "tiki/container/array.hpp"
#include "tiki/container/list.hpp"
#include "tiki/math/matrix.hpp"
#include "tiki/math/quaternion.hpp"
#include "tiki/math/vector.hpp"

namespace tiki
{
	class ResourceWriter;
	class ResourceSectionWriter;
	struct ReferenceKey;

	class AnimationBuilder
	{
	public:

		bool								create( uint lengthInFrames, uint jointCount, uint framesPerChunk, float constEpsilon );
		void								dispose();

		void								setJointData( uint jointIndex, const AnimationSpline& Rotation, const AnimationSpline& position, const AnimationSpline& scale, const Matrix44& defaultPose );
		void								setConstantJointData( uint jointIndex, const Quaternion& Rotation, const Vector3& position, const Vector3& scale, const Matrix44& defaultPose );

		void								writeToResource( ResourceWriter& resourceWriter, ReferenceKey& targetKey );

	private:

		struct ChunkHeader
		{
			uint16	startTime;
			uint16	endTime;

			uint	dataOffset;

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

		struct DecomposedValue
		{
			float aValues[ 4u ];
		};

		uint								m_jointCount;
		uint								m_lengthInFrames;
		float								m_constEpsilon;

		float								m_maxPositionValue;
		float								m_maxPositionTangent;
		float								m_maxScaleValue;
		float								m_maxScaleTangent;

		List< ChunkHeader >					m_headers;

		Array< List< AnimationSplineKey > >	m_jointRotationKeys;
		Array< List< AnimationSplineKey > >	m_jointPositionKeys;
		Array< List< AnimationSplineKey > >	m_jointScaleKeys;

		Array< DecomposedValue >			m_defaultPoseRotation;
		Array< DecomposedValue >			m_defaultPosePosition;
		Array< DecomposedValue >			m_defaultPoseScale;

		void								writeChunkData( ResourceSectionWriter& sectionWriter, uint16& interpolatedJointCount, uint16& usedJointCount, uint16& defaultPoseJointCount, const Array< List< AnimationSplineKey > >& sourceData, const Array< DecomposedValue >& defaultPose, ChunkHeader& header, uint dimensions, bool normalizeValue, float factor, float factorTangent );
		void								fillDataArray( List< sint16 >& target, const AnimationSplineKey& sourceKey, uint dimensions, float factor, float factorTangent, bool isConstKey );
		AnimationSplineKey					createInterpolatedKey( const AnimationSplineKey& leftKey, const AnimationSplineKey& rightKey, uint time, uint dimensions, bool normalizeValue );

		uint64								generateBitMaskAndFindKeys( List< AnimationSplineKey >& targetList, const List< AnimationSplineKey >& sourceKeys, uint16 startTime, uint16 endTime, uint dimensions, bool normalizeValue );
		void								insertBitMask( List< AnimationSplineKey >& targetList, uint64& mask, const AnimationSplineKey& key, uint16 startTime );

		float								getPositionFactor() const			{ return m_maxPositionValue / 32500.0f; }
		float								getPositionTangentFactor() const	{ return m_maxPositionTangent / 32500.0f; }
		float								getScaleFactor() const				{ return m_maxScaleValue / 32500.0f; }
		float								getScaleTangentFactor() const		{ return m_maxScaleTangent / 32500.0f; }
	};
}
