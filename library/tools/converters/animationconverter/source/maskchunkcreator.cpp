
#include "tiki/animationconverter/maskchunkcreator.hpp"
#include "tiki/base/assert.hpp"
#include "tiki/base/float32.hpp"

namespace tiki
{
	static const float s_jointOrientationFactor	= 3.0518513e-005f;
	static const float s_jointTangentFactor		= 7.6293945e-006f;

	void MaskChunkCreator::create( uint16 jointCount, uint16 framesPerChunk, uint16 lengthInFrames, float constEpsilon )
	{
		TIKI_ASSERT( framesPerChunk <= 64 );
		
		uint16 chunkCount = (uint16)((lengthInFrames + (framesPerChunk - 2u)) / (framesPerChunk - 1u));

		m_constEpsilon			= constEpsilon;
		m_maxPositionValue		= 0.0f;
		m_maxPositionTangent	= 0.0f;
		m_maxScaleValue			= 0.0f;

		m_headers.create( chunkCount );

		uint16 lastEnd = 0u;
		for (size_t i = 0; i < chunkCount; ++i)
		{
			MaskChunkHeader header;

			header.startTime = lastEnd;
			header.endTime = (uint16)(header.startTime + (framesPerChunk - 1u));
			header.dataOffset = 0;
			header.usedOrientationJoints = 0;
			header.usedPositionJoints = 0;
			header.usedScaleJoints = 0;
			header.nonConstOrientationJoints = 0;
			header.nonConstPositionJoints = 0;
			header.nonConstScaleJoints = 0;

			m_headers[ i ]	= header;
			lastEnd			= header.endTime;
		}

		m_jointCount = jointCount;

		m_jointOrientationKeys.create( jointCount );
		m_jointPositionKeys.create( jointCount );
		m_jointScaleKeys.create( jointCount );

		m_defaultPoseOrientation.create( jointCount );
		memory::zero( m_defaultPoseOrientation.getData(), jointCount * sizeof( DecomposedValue ) );

		m_defaultPosePosition.create( jointCount );
		memory::zero( m_defaultPosePosition.getData(), jointCount * sizeof( DecomposedValue ) );

		m_defaultPoseScale.create( jointCount );
		memory::zero( m_defaultPoseScale.getData(), jointCount * sizeof( DecomposedValue ) );
	}

	void MaskChunkCreator::dispose()
	{
		for (size_t i = 0; i < m_dataBlocks.getCount(); ++i)
		{
			if (m_dataBlocks[ i ].pData != nullptr)
			{
				memory::freeAlign( m_dataBlocks[ i ].pData );
			}
		}

		m_headers.dispose();

		m_jointOrientationKeys.dispose();
		m_jointPositionKeys.dispose();
		m_jointScaleKeys.dispose();

		m_defaultPoseOrientation.dispose();
		m_defaultPosePosition.dispose();
		m_defaultPoseScale.dispose();
	}

	void MaskChunkCreator::setJointData( uint16 jointIndex, const Spline& orientation, const Spline& position, const Spline& scale, const Matrix& defaultPose )
	{
		TIKI_ASSERT( jointIndex < m_jointCount );
		TIKI_ASSERT( m_jointOrientationKeys.getCount() != 0 );
		TIKI_ASSERT( m_jointPositionKeys.getCount() != 0 );
		TIKI_ASSERT( m_jointScaleKeys.getCount() != 0 );

		List< SplineKey >& orientationKeys	= m_jointOrientationKeys[ jointIndex ];
		List< SplineKey >& positionKeys		= m_jointPositionKeys[ jointIndex ];
		List< SplineKey >& scaleKeys		= m_jointScaleKeys[ jointIndex ];

		Vector3 vecPos;
		Vector3 vecScale;
		Quaternion quatRot;
		defaultPose.decompose( quatRot, vecPos, vecScale );

		m_defaultPoseOrientation[ jointIndex ].value[ 0u ] = quatRot.x;
		m_defaultPoseOrientation[ jointIndex ].value[ 1u ] = quatRot.y;
		m_defaultPoseOrientation[ jointIndex ].value[ 2u ] = quatRot.z;
		m_defaultPoseOrientation[ jointIndex ].value[ 3u ] = quatRot.w;

		m_defaultPosePosition[ jointIndex ].value[ 0u ] = vecPos.x;
		m_defaultPosePosition[ jointIndex ].value[ 1u ] = vecPos.y;
		m_defaultPosePosition[ jointIndex ].value[ 2u ] = vecPos.z;

		m_defaultPoseScale[ jointIndex ].value[ 0u ] = vecScale.x;
		m_defaultPoseScale[ jointIndex ].value[ 1u ] = vecScale.y;
		m_defaultPoseScale[ jointIndex ].value[ 2u ] = vecScale.z;

		TIKI_ASSERT( orientationKeys.getCount() == 0u );
		TIKI_ASSERT( positionKeys.getCount() == 0u );
		TIKI_ASSERT( scaleKeys.getCount() == 0u );

		orientationKeys.reserve( orientation.keyData.getCount() );
		for (size_t i = 0; i < orientation.keyData.getCount(); ++i)
		{
			orientationKeys.add( orientation.keyData[ i ] );
		}

		positionKeys.reserve( position.keyData.getCount() );
		for (size_t i = 0; i < position.keyData.getCount(); ++i)
		{
			positionKeys.add( position.keyData[ i ] );

			for (size_t j = 0; j < 3u; ++j)
			{
				m_maxPositionValue		= TIKI_MAX( m_maxPositionValue, abs( position.keyData[ i ].values[ j ] ) );
				m_maxPositionTangent	= TIKI_MAX( m_maxPositionTangent, abs( position.keyData[ i ].controls[ j ] ) );
			}
		}

		scaleKeys.reserve( scale.keyData.getCount() );
		for (size_t i = 0; i < scale.keyData.getCount(); ++i)
		{
			scaleKeys.add( scale.keyData[ i ] );

			for (size_t j = 0; j < 3u; ++j)
			{
				m_maxScaleValue		= TIKI_MAX( m_maxScaleValue, abs( scale.keyData[ i ].values[ j ] ) );
			}
		}
	}

	void MaskChunkCreator::setConstantJoint( uint16 jointIndex, const Quaternion& orientation, const Vector3& position, const Vector3& scale, const Matrix& defaultPose )
	{
		TIKI_ASSERT( jointIndex < m_jointCount );

		SplineKey orientationKey;
		orientationKey.time = 0;
		orientationKey.values[ 0u ] = orientation.x;
		orientationKey.values[ 1u ] = orientation.y;
		orientationKey.values[ 2u ] = orientation.z;
		orientationKey.values[ 3u ] = orientation.w;
		m_jointOrientationKeys[ jointIndex ].add( orientationKey );

		SplineKey positionKey;
		positionKey.time = 0;
		positionKey.values[ 0u ] = position.x;
		positionKey.values[ 1u ] = position.y;
		positionKey.values[ 2u ] = position.z;
		m_jointPositionKeys[ jointIndex ].add( positionKey );

		SplineKey scaleKey;
		scaleKey.time = 0;
		scaleKey.values[ 0u ] = scale.x;
		scaleKey.values[ 1u ] = scale.y;
		scaleKey.values[ 2u ] = scale.z;
		m_jointScaleKeys[ jointIndex ].add( scaleKey );

		Quaternion orientation2;
		Vector3 position2;
		Vector3 scale2;
		defaultPose.decompose( orientation2, position2, scale2 );

		m_defaultPoseOrientation[ jointIndex ].value[ 0u ] = orientation2.x;
		m_defaultPoseOrientation[ jointIndex ].value[ 1u ] = orientation2.y;
		m_defaultPoseOrientation[ jointIndex ].value[ 2u ] = orientation2.z;
		m_defaultPoseOrientation[ jointIndex ].value[ 3u ] = orientation2.w;

		m_defaultPosePosition[ jointIndex ].value[ 0u ] = position2.x;
		m_defaultPosePosition[ jointIndex ].value[ 1u ] = position2.y;
		m_defaultPosePosition[ jointIndex ].value[ 2u ] = position2.z;

		m_defaultPoseScale[ jointIndex ].value[ 0u ] = scale2.x;
		m_defaultPoseScale[ jointIndex ].value[ 1u ] = scale2.y;
		m_defaultPoseScale[ jointIndex ].value[ 2u ] = scale2.z;
	}

	void MaskChunkCreator::finalizeData()
	{
		size_t offset = 0u;

		for (size_t i = 0; i < m_headers.getCount(); ++i)
		{
			MaskChunkHeader* pHeader = &m_headers[ i ];
			pHeader->dataOffset = alignValue< size_t >( offset, 8u );

			chunksFillData(
				&pHeader->nonConstOrientationJoints,
				&pHeader->usedOrientationJoints,
				m_jointOrientationKeys,
				m_defaultPoseOrientation,
				pHeader,
				4u,
				true,
				s_jointOrientationFactor,
				s_jointTangentFactor
			);

			chunksFillData(
				&pHeader->nonConstPositionJoints,
				&pHeader->usedPositionJoints,
				m_jointPositionKeys,
				m_defaultPosePosition,
				pHeader,
				3u,
				false,
				getPositionFactor(),
				getPositionTangent()
			);

			chunksFillData(
				&pHeader->nonConstScaleJoints,
				&pHeader->usedScaleJoints,
				m_jointScaleKeys,
				m_defaultPoseScale,
				pHeader,
				3u,
				false,
				getScaleFactor(),
				s_jointTangentFactor
			);

			offset = 0u;
			for (size_t i = 0; i < m_dataBlocks.getCount(); ++i)
			{
				const AnimationDataBlobArray& dataBlock = m_dataBlocks[ i ];

				if (dataBlock.alignment)
				{
					offset = alignValue( offset, dataBlock.alignment );
				}

				offset += dataBlock.elementCount * dataBlock.typeSize;
			}
		}

		m_dataSize = offset;
	}

	const MaskChunkHeader* MaskChunkCreator::getChunkHeaders() const
	{
		return m_headers.getData(); 
	}

	size_t MaskChunkCreator::getChunkHeaderCount() const
	{
		return m_headers.getCount();
	}

	const AnimationDataBlobArray* MaskChunkCreator::getDataBlocks() const
	{
		return m_dataBlocks.getData();
	}

	size_t MaskChunkCreator::getDataBlockCount() const
	{
		return m_dataBlocks.getCount();
	}

	size_t MaskChunkCreator::getDataSize() const
	{
		return m_dataSize;
	}

	void MaskChunkCreator::chunksFillData( uint16* pNonConst, uint16* pUsed, const Array< List< SplineKey > >& sourceData, const Array< DecomposedValue >& defaultPose, MaskChunkHeader* pHeader, uint dimentions, bool normalizeValue, float factor, float factorTangent )
	{
		List< SplineKey > source;

		uint16 nonConst = 0;
		uint16 usedJoints = 0;

		List< uint64 > masks;
		List< sint16 > keyData;
		List< sint16 > constKeyData;
		List< uint16 > unusedJoints;

		unusedJoints.add( 0 );

		for (size_t j = 0; j < m_jointCount; ++j)
		{
			source.clear();

			uint64 mask = bitMaskAndKeyFinding( source, sourceData[ j ], pHeader->startTime, pHeader->endTime, dimentions, normalizeValue );

			if (source.getCount() == 0)
			{
				unusedJoints.add( (uint16)j );
				continue;
			}
			else
			{
				bool constJoint = source.getCount() <= 2u;
				if (constJoint && source.getCount() == 2u)
				{
					const SplineKey& key0 = source[ 0u ];
					const SplineKey& key1 = source[ 1u ];

					for (size_t i = 0; i < dimentions; ++i)
					{
						if (!f32::isEquals( key0.values[ i ], key1.values[ i ], m_constEpsilon ) )
						{
							constJoint = false;
							break;
						}
					}
				}

				if (constJoint)
				{
					bool isEquals = true;
					for (size_t i = 0; i < dimentions; ++i)
					{
						if (!f32::isEquals( defaultPose[ j ].value[ i ], source[ 0 ].values[ i ], m_constEpsilon ) )
						{
							isEquals = false;
							break;
						}
					}
					
					if (isEquals)
					{
						unusedJoints.add( (uint16)j );
						continue;
					}

					constKeyData.add( (sint16)j );
					chunksWriteData( constKeyData, source[ 0u ], dimentions, factor, factorTangent, true );
				}
				else
				{
					TIKI_ASSERT( mask > 0 );

					masks.add(mask);
					keyData.add( (sint16)j );

					for (size_t k = 0; k < source.getCount(); ++k)
					{
						chunksWriteData( keyData, source[ k ], dimentions, factor, factorTangent, false );
					}

					nonConst++;
				}
			}

			usedJoints++;
		}

		*pNonConst = nonConst;
		*pUsed = usedJoints;

		unusedJoints[ 0u ] = (uint16)(unusedJoints.getCount() - 1u);

		chunksWriteChannel( masks, keyData, constKeyData, unusedJoints );
	}

	void MaskChunkCreator::chunksWriteData( List< sint16 >& target, const SplineKey& source, uint dimentions, float factor, float factorTangent, bool constKey )
	{
		const sint16 values[] =
		{
			(sint16)(source.values[ 0u ] / factor),
			(sint16)(source.values[ 1u ] / factor),
			(sint16)(source.values[ 2u ] / factor),
			(sint16)(source.values[ 3u ] / factor)
		};

		for (size_t i = 0; i < dimentions; ++i)
		{
			target.add( values[ i ] );
		}

		if (!constKey)
		{
			const sint16 controls[] =
			{
				(sint16)(source.controls[ 0u ] / factorTangent),
				(sint16)(source.controls[ 1u ] / factorTangent),
				(sint16)(source.controls[ 2u ] / factorTangent),
				(sint16)(source.controls[ 3u ] / factorTangent)
			};

			for (size_t i = 0; i < dimentions; ++i)
			{
				target.add( controls[ i ] );
			}
		}
	}

	void MaskChunkCreator::chunksWriteChannel( const List< uint64 >& masks, const List< sint16 >& keyData, const List< sint16 >& constKeyData, const List< uint16 >& unusedJoints )
	{
		addBlock(
			8u,
			DataType_UInt64,
			masks.getData(),
			masks.getCount()
		);

		if (keyData.getCount() > 0u)
		{
			addBlock(
				0u,
				DataType_SInt16,
				keyData.getData(),
				keyData.getCount()
			);
		}

		if (constKeyData.getCount() > 0u)
		{
			addBlock(
				0u,
				DataType_SInt16,
				constKeyData.getData(),
				constKeyData.getCount()
			);
		}

		addBlock(
			0u,
			DataType_UInt16,
			unusedJoints.getData(),
			unusedJoints.getCount()
		);
	}

	SplineKey MaskChunkCreator::chunksInterpolateKey( const SplineKey& leftKey, const SplineKey& rightKey, uint time, uint dimentions, bool normalizeValue )
	{
		const float tscale = (float)(rightKey.time - leftKey.time);
		const float koeff = (float)(time - leftKey.time) / tscale;

		SplineKey key;
		key.time = (uint16)time;

		sampleSpline( key.values, dimentions, leftKey.values, rightKey.values, leftKey.controls, rightKey.controls, tscale, koeff );

		for (size_t i = 0; i < dimentions; ++i)
		{
			key.controls[ i ] = leftKey.controls[ i ] + ((rightKey.controls[ i ] - leftKey.controls[ i ]) * koeff);
		}

		if (normalizeValue)
		{
			float lengthSquad = 0.0f;
			for (size_t i = 0; i < dimentions; ++i)
			{
				lengthSquad += key.values[ i ] * key.values[ i ];
			}
			const float length = 1.0f / sqrt( lengthSquad );

			for (size_t i = 0; i < dimentions; ++i)
			{
				key.values[ i ] *= length;
			}
		}

		return key;
	}

	uint64 MaskChunkCreator::bitMaskAndKeyFinding( List< SplineKey >& targetList, const List< SplineKey >& sourceKeys, uint16 startTime, uint16 endTime, uint dimentions, bool normalizeValue )
	{
		if (sourceKeys.getCount() == 0u)
		{
			return 0u;
		}
		if (sourceKeys.getCount() == 1u)
		{
			targetList.add( sourceKeys[ 0u ] );
			return 0u;
		}
		else if (sourceKeys.getCount() == 2u)
		{
			const float* f1 = sourceKeys[ 0 ].values;
			const float* f2 = sourceKeys[ 1 ].values;

			bool equals = true;

			for (size_t i = 0; i < dimentions; ++i)
			{
				if (!f32::isEquals( f1[ i ], f2[ i ], 0.0001f ))
				{
					equals = false;
					break;
				}
			}

			if (equals)
			{
				targetList.add( sourceKeys[ 0u ] );
				return 0u;
			}
		}

		uint32 firstIndex = 0xffffffff;
		uint32 lastIndex = 0xffffffff;
		SplineKey firstKey;
		SplineKey lastKey;

		for (uint32 i = 0; i < sourceKeys.getCount(); i++)
		{
			if (firstIndex == 0xffffffff)
			{
				if (sourceKeys[ i ].time > startTime)
				{
					firstIndex = i - 1;
				}
				else if (sourceKeys[ i ].time == startTime)
				{
					firstIndex = i;
				}
			}

			if (sourceKeys[ i ].time >= endTime || i == sourceKeys.getCount() - 1)
			{
				lastIndex = i;
				break;
			}
		}
		TIKI_ASSERT( firstIndex != 0xffffffff );
		
		if (sourceKeys[ firstIndex ].time != startTime)
		{
			firstKey = chunksInterpolateKey( sourceKeys[ firstIndex ], sourceKeys[ firstIndex + 1 ], startTime, dimentions, normalizeValue );
		}
		else
		{
			firstKey = sourceKeys[ firstIndex ];
		}
		firstIndex++;

		SplineKey sourceLastKey = sourceKeys[ lastIndex ];
		if (sourceKeys[ lastIndex ].time != endTime)
		{
			lastKey = chunksInterpolateKey( sourceKeys[ lastIndex - 1 ], sourceKeys[ lastIndex ], endTime, dimentions, normalizeValue );
		}
		else
		{
			lastKey = sourceKeys[ lastIndex ];
		}

		uint64 mask = 0;

		bitMaskInsert( targetList, mask, firstKey, startTime );
		for (uint32 i = firstIndex; i < lastIndex; i++)
		{
			bitMaskInsert( targetList, mask, sourceKeys[ i ], startTime );
		}

		if (sourceLastKey.time <= endTime && sourceLastKey.time != startTime)
		{
			bitMaskInsert( targetList, mask, sourceLastKey, startTime );
		}

		if (lastKey.time < sourceLastKey.time)
		{
			bitMaskInsert( targetList, mask, lastKey, startTime );
		}
		
		if ( (mask & 0x8000000000000001u) != 0x8000000000000001u && sourceLastKey.time >= endTime )
		{
			TIKI_TRACE_ERROR( "incomplete bit mask in chunk: %i", (startTime / (endTime - startTime)) );
		}

		return mask;
	}

	void MaskChunkCreator::bitMaskInsert( List< SplineKey >& targetList, uint64& mask, const SplineKey& key, uint16 startTime )
	{
		const uint64 orMask = ((uint64)1u << (key.time - startTime));
		
		if ( (mask & orMask) != 0u )
		{
			TIKI_TRACE_ERROR( "bit mask overlapping. converter bug?" );
		}

		mask |= orMask;
		targetList.add( key );
	}

	void MaskChunkCreator::addBlock( size_t alignment, AnimationDataBlobArrayType type, const void* pData, size_t elementCount )
	{
		AnimationDataBlobArray block;
		block.alignment = alignment;
		block.elementCount = elementCount;
		block.type = type;
		block.typeSize = 0u;
		
		switch (type)
		{
		case DataType_SInt8:
		case DataType_UInt8:
			block.typeSize = 1u;
			break;
		case DataType_SInt16:
		case DataType_UInt16:
			block.typeSize = 2u;
			break;
		case DataType_SInt32:
		case DataType_UInt32:
			block.typeSize = 4u;
			break;
		case DataType_SInt64:
		case DataType_UInt64:
			block.typeSize = 8u;
			break;
		}

		size_t dataSize = elementCount * block.typeSize;
		if (dataSize > 0)
		{
			block.pData = (uint8*)memory::allocAlign( dataSize, 16u );
			memory::copy( block.pData, pData, dataSize );
		}
		else
		{
			block.pData = nullptr;
		}

		m_dataBlocks.add( block );
	}
}
