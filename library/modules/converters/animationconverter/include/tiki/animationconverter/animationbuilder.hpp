#pragma once
#ifndef __TIKI_ANIMATIONBUILDER_HPP_INCLUDED__
#define __TIKI_ANIMATIONBUILDER_HPP_INCLUDED__

#include "tiki/animationconverter/spline.hpp"
#include "tiki/base/array.hpp"
#include "tiki/base/types.hpp"
#include "tiki/math/matrix.hpp"
#include "tiki/math/quaternion.hpp"
#include "tiki/math/vector.hpp"
#include "tiki/toolbase/list.hpp"

namespace tiki
{
	class ResourceWriter;
	struct ReferenceKey;

	class AnimationBuilder
	{
	public:
				
		bool								create( uint lengthInFrames, uint jointCount, uint framesPerChunk, float constEpsilon );
		void								dispose();

		void								setJointData( uint jointIndex, const Spline& Rotation, const Spline& position, const Spline& scale, const Matrix44& defaultPose );
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

		Array< List< SplineKey > >			m_jointRotationKeys;
		Array< List< SplineKey > >			m_jointPositionKeys;
		Array< List< SplineKey > >			m_jointScaleKeys;

		Array< DecomposedValue >			m_defaultPoseRotation;
		Array< DecomposedValue >			m_defaultPosePosition;
		Array< DecomposedValue >			m_defaultPoseScale;

		void								writeChunkData( ResourceWriter& resourceWriter, uint16& interpolatedJointCount, uint16& usedJointCount, uint16& defaultPoseJointCount, const Array< List< SplineKey > >& sourceData, const Array< DecomposedValue >& defaultPose, ChunkHeader& header, uint dimensions, bool normalizeValue, float factor, float factorTangent );
		void								fillDataArray( List< sint16 >& target, const SplineKey& sourceKey, uint dimensions, float factor, float factorTangent, bool isConstKey );
		SplineKey							createInterpolatedKey( const SplineKey& leftKey, const SplineKey& rightKey, uint time, uint dimensions, bool normalizeValue );

		uint64								generateBitMaskAndFindKeys( List< SplineKey >& targetList, const List< SplineKey >& sourceKeys, uint16 startTime, uint16 endTime, uint dimensions, bool normalizeValue );
		void								insertBitMask( List< SplineKey >& targetList, uint64& mask, const SplineKey& key, uint16 startTime );

		float								getPositionFactor() const			{ return m_maxPositionValue / 32500.0f; }
		float								getPositionTangentFactor() const	{ return m_maxPositionTangent / 32500.0f; }
		float								getScaleFactor() const				{ return m_maxScaleValue / 32500.0f; }
		float								getScaleTangentFactor() const		{ return m_maxScaleTangent / 32500.0f; }

	};
}

#endif // __TIKI_ANIMATIONBUILDER_HPP_INCLUDED__
