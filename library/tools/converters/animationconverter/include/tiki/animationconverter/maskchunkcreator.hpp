#ifndef TIKI_MASKCHUNKCREATOR_HPP
#define TIKI_MASKCHUNKCREATOR_HPP

#include "tiki/animationconverter/datablock.hpp"
#include "tiki/animationconverter/splinefitter.hpp"
#include "tiki/base/array.hpp"
#include "tiki/base/list.hpp"
#include "tiki/base/types.hpp"
#include "tiki/math/matrix.hpp"
#include "tiki/math/quaternion.hpp"
#include "tiki/math/vector3.hpp"

namespace tiki
{
	struct MaskChunkHeader
	{
		uint16	startTime;
		uint16	endTime;

		size_t	dataOffset;

		uint16	usedOrientationJoints;
		uint16	nonConstOrientationJoints;

		uint16	usedPositionJoints;
		uint16	nonConstPositionJoints;

		uint16	usedScaleJoints;
		uint16	nonConstScaleJoints;
	};

	class MaskChunkCreator
	{
	public:

		// max 64 frames per chunk
		void								create( uint16 jointCount, uint16 framesPerChunk, uint16 lengthInFrames, float constEpsilon );
		void								dispose();

		void								setJointData( uint16 jointIndex, const Spline& orientation, const Spline& position, const Spline& scale, const Matrix& defaultPose );
		void								setConstantJoint( uint16 jointIndex, const Quaternion& orientation, const Vector3& position, const Vector3& scale, const Matrix& defaultPose );

		void								finalizeData();

		const MaskChunkHeader*				getChunkHeaders() const;
		size_t								getChunkHeaderCount() const;

		const AnimationDataBlobArray*		getDataBlocks() const;
		size_t								getDataBlockCount() const;
		size_t								getDataSize() const;

		float								getPositionFactor() const { return m_maxPositionValue / 32500.0f; }
		float								getPositionTangent() const { return m_maxPositionTangent / 32500.0f; }
		float								getScaleFactor() const { return m_maxScaleValue / 32500.0f; }

	private:

		struct DecomposedValue
		{
			float	value[ 4u ];
		};

		size_t								m_dataSize;
		float								m_constEpsilon;

		float								m_maxPositionValue;
		float								m_maxPositionTangent;
		float								m_maxScaleValue;

		Array< MaskChunkHeader >			m_headers;
		List< AnimationDataBlobArray >		m_dataBlocks;

		uint16								m_jointCount;
		Array< List< SplineKey > >			m_jointOrientationKeys;
		Array< List< SplineKey > >			m_jointPositionKeys;
		Array< List< SplineKey > >			m_jointScaleKeys;

		Array< DecomposedValue >			m_defaultPoseOrientation;
		Array< DecomposedValue >			m_defaultPosePosition;
		Array< DecomposedValue >			m_defaultPoseScale;

		void								chunksFillData( uint16* pNonConst, uint16* pUsed, const Array< List< SplineKey > >& sourceData, const Array< DecomposedValue >& defaultPose, MaskChunkHeader* pHeader, uint dimentions, bool normalizeValue, float factor, float factorTangent );
		void								chunksWriteData( List< sint16 >& target, const SplineKey& source, uint dimentions, float factor, float factorTangent, bool constKey );
		void								chunksWriteChannel( const List< uint64 >& masks, const List< sint16 >& keyData, const List< sint16 >& constKeyData, const List< uint16 >& unusedJoints );
		SplineKey							chunksInterpolateKey( const SplineKey& leftKey, const SplineKey& rightKey, uint time, uint dimentions, bool normalizeValue );

		uint64								bitMaskAndKeyFinding( List< SplineKey >& targetList, const List< SplineKey >& sourceKeys, uint16 startTime, uint16 endTime, uint dimentions, bool normalizeValue );
		void								bitMaskInsert( List< SplineKey >& targetList, uint64& mask, const SplineKey& key, uint16 startTime );

		void								addBlock( size_t alignment, AnimationDataBlobArrayType type, const void* pData, size_t elementCount );

	};
}

#endif // TIKI_CHUNKCREATOR_HPP
