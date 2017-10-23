#include "tiki/animationconverter/animation_builder.hpp"

#include "tiki/animationconverter/spline.hpp"
#include "tiki/base/assert.hpp"
#include "tiki/base/float32.hpp"
#include "tiki/converterbase/resource_writer.hpp"

namespace tiki
{
	static const float s_jointRotationFactor		= 3.0518513e-005f;
	static const float s_jointRotationTangentFactor	= 7.6293945e-006f;

	bool AnimationBuilder::create( uint lengthInFrames, uint jointCount, uint framesPerChunk, float constEpsilon )
	{
		TIKI_ASSERT( framesPerChunk <= 64 );

		uint16 chunkCount = (uint16)((lengthInFrames + (framesPerChunk - 2u)) / (framesPerChunk - 1u));

		m_jointCount			= jointCount;
		m_lengthInFrames		= lengthInFrames;
		m_constEpsilon			= constEpsilon;

		m_maxPositionValue		= 0.0f;
		m_maxPositionTangent	= 0.0f;
		m_maxScaleValue			= 0.0f;
		m_maxScaleTangent		= 0.0f;

		uint16 lastEndTime = 0u;
		for (uint i = 0; i < chunkCount; ++i)
		{
			ChunkHeader& header = m_headers.add();
			header.startTime	= lastEndTime;
			header.endTime		= (uint16)(header.startTime + (framesPerChunk - 1u));
			header.dataOffset	= 0u;

			header.usedRotationJointCount			= 0u;
			header.interpolatedRotationJointCount	= 0u;
			header.defaultPoseRotationJointCount	= 0u;
			header.usedPositionJointCount			= 0u;
			header.interpolatedPositionJointCount	= 0u;
			header.defaultPosePositionJointCount	= 0u;
			header.usedScaleJointCount				= 0u;
			header.interpolatedScaleJointCount		= 0u;
			header.defaultPoseScaleJointCount		= 0u;

			lastEndTime = header.endTime;
		}

		m_jointRotationKeys.create( jointCount );
		m_jointPositionKeys.create( jointCount );
		m_jointScaleKeys.create( jointCount );

		m_defaultPoseRotation.create( jointCount );
		m_defaultPosePosition.create( jointCount );
		m_defaultPoseScale.create( jointCount );
		memory::zero( m_defaultPoseRotation.getBegin(), jointCount * sizeof( DecomposedValue ) );
		memory::zero( m_defaultPosePosition.getBegin(), jointCount * sizeof( DecomposedValue ) );
		memory::zero( m_defaultPoseScale.getBegin(), jointCount * sizeof( DecomposedValue ) );

		return true;
	}

	void AnimationBuilder::dispose()
	{
		m_headers.dispose();

		m_jointRotationKeys.dispose();
		m_jointPositionKeys.dispose();
		m_jointScaleKeys.dispose();

		m_defaultPoseRotation.dispose();
		m_defaultPosePosition.dispose();
		m_defaultPoseScale.dispose();
	}

	void AnimationBuilder::setJointData( uint jointIndex, const Spline& rotation, const Spline& position, const Spline& scale, const Matrix44& defaultPose )
	{
		TIKI_ASSERT( jointIndex < m_jointCount );
		TIKI_ASSERT( m_jointRotationKeys.getCount() != 0 );
		TIKI_ASSERT( m_jointPositionKeys.getCount() != 0 );
		TIKI_ASSERT( m_jointScaleKeys.getCount() != 0 );

		List< SplineKey >& rotationKeys	= m_jointRotationKeys[ jointIndex ];
		List< SplineKey >& positionKeys	= m_jointPositionKeys[ jointIndex ];
		List< SplineKey >& scaleKeys	= m_jointScaleKeys[ jointIndex ];

		Quaternion defaultRotation;
		Vector3 defaultPosition;
		Vector3 defaultScale;
		matrix::decompose( defaultRotation, defaultPosition, defaultScale, defaultPose );

		m_defaultPoseRotation[ jointIndex ].aValues[ 0u ] = defaultRotation.x;
		m_defaultPoseRotation[ jointIndex ].aValues[ 1u ] = defaultRotation.y;
		m_defaultPoseRotation[ jointIndex ].aValues[ 2u ] = defaultRotation.z;
		m_defaultPoseRotation[ jointIndex ].aValues[ 3u ] = defaultRotation.w;

		m_defaultPosePosition[ jointIndex ].aValues[ 0u ] = defaultPosition.x;
		m_defaultPosePosition[ jointIndex ].aValues[ 1u ] = defaultPosition.y;
		m_defaultPosePosition[ jointIndex ].aValues[ 2u ] = defaultPosition.z;

		m_defaultPoseScale[ jointIndex ].aValues[ 0u ] = defaultScale.x;
		m_defaultPoseScale[ jointIndex ].aValues[ 1u ] = defaultScale.y;
		m_defaultPoseScale[ jointIndex ].aValues[ 2u ] = defaultScale.z;

		TIKI_ASSERT( rotationKeys.getCount() == 0u );
		TIKI_ASSERT( positionKeys.getCount() == 0u );
		TIKI_ASSERT( scaleKeys.getCount() == 0u );

		rotationKeys.reserve( rotation.getKeyCount() );
		for (uint i = 0; i < rotation.getKeyCount(); ++i)
		{
			rotationKeys.add( rotation.getKeyByIndex( i ) );
		}

		positionKeys.reserve( position.getKeyCount() );
		for (uint i = 0; i < position.getKeyCount(); ++i)
		{
			positionKeys.add( position.getKeyByIndex( i ) );

			for (uint j = 0; j < 3u; ++j)
			{
				m_maxPositionValue		= TIKI_MAX( m_maxPositionValue, fabsf( position.getKeyByIndex( i ).aValues[ j ] ) );
				m_maxPositionTangent	= TIKI_MAX( m_maxPositionTangent, fabsf( position.getKeyByIndex( i ).aControls[ j ] ) );
			}
		}

		scaleKeys.reserve( scale.getKeyCount() );
		for (uint i = 0; i < scale.getKeyCount(); ++i)
		{
			scaleKeys.add( scale.getKeyByIndex( i ) );

			for (uint j = 0; j < 3u; ++j)
			{
				m_maxScaleValue		= TIKI_MAX( m_maxScaleValue, fabsf( scale.getKeyByIndex( i ).aValues[ j ] ) );
				m_maxScaleTangent	= TIKI_MAX( m_maxScaleTangent, fabsf( scale.getKeyByIndex( i ).aControls[ j ] ) );
			}
		}
	}

	void AnimationBuilder::setConstantJointData( uint jointIndex, const Quaternion& rotation, const Vector3& position, const Vector3& scale, const Matrix44& defaultPose )
	{
		TIKI_ASSERT( jointIndex < m_jointCount );

		SplineKey rotationKey;
		rotationKey.time = 0u;
		rotationKey.aValues[ 0u ] = rotation.x;
		rotationKey.aValues[ 1u ] = rotation.y;
		rotationKey.aValues[ 2u ] = rotation.z;
		rotationKey.aValues[ 3u ] = rotation.w;
		m_jointRotationKeys[ jointIndex ].add( rotationKey );

		SplineKey positionKey;
		positionKey.time = 0u;
		positionKey.aValues[ 0u ] = position.x;
		positionKey.aValues[ 1u ] = position.y;
		positionKey.aValues[ 2u ] = position.z;
		m_jointPositionKeys[ jointIndex ].add( positionKey );

		SplineKey scaleKey;
		scaleKey.time = 0u;
		scaleKey.aValues[ 0u ] = scale.x;
		scaleKey.aValues[ 1u ] = scale.y;
		scaleKey.aValues[ 2u ] = scale.z;
		m_jointScaleKeys[ jointIndex ].add( scaleKey );

		Quaternion defaultRotation;
		Vector3 defaultPosition;
		Vector3 defaultScale;
		matrix::decompose( defaultRotation, defaultPosition, defaultScale, defaultPose );

		m_defaultPoseRotation[ jointIndex ].aValues[ 0u ] = defaultRotation.x;
		m_defaultPoseRotation[ jointIndex ].aValues[ 1u ] = defaultRotation.y;
		m_defaultPoseRotation[ jointIndex ].aValues[ 2u ] = defaultRotation.z;
		m_defaultPoseRotation[ jointIndex ].aValues[ 3u ] = defaultRotation.w;

		m_defaultPosePosition[ jointIndex ].aValues[ 0u ] = defaultPosition.x;
		m_defaultPosePosition[ jointIndex ].aValues[ 1u ] = defaultPosition.y;
		m_defaultPosePosition[ jointIndex ].aValues[ 2u ] = defaultPosition.z;

		m_defaultPoseScale[ jointIndex ].aValues[ 0u ] = defaultScale.x;
		m_defaultPoseScale[ jointIndex ].aValues[ 1u ] = defaultScale.y;
		m_defaultPoseScale[ jointIndex ].aValues[ 2u ] = defaultScale.z;
	}

	void AnimationBuilder::writeToResource( ResourceWriter& resourceWriter, ReferenceKey& targetKey )
	{
		ResourceSectionWriter sectionWriter;
		resourceWriter.openDataSection(  sectionWriter, SectionType_Main, 16u );
		const ReferenceKey animationDataKey = sectionWriter.addDataPoint();

		for (uint i = 0; i < m_headers.getCount(); ++i)
		{
			ChunkHeader& header = m_headers[ i ];
			header.dataOffset = sectionWriter.getCurrentSize();

			writeChunkData(
				sectionWriter,
				header.interpolatedRotationJointCount,
				header.usedRotationJointCount,
				header.defaultPoseRotationJointCount,
				m_jointRotationKeys,
				m_defaultPoseRotation,
				header,
				4u,
				true,
				s_jointRotationFactor,
				s_jointRotationTangentFactor
			);

			writeChunkData(
				sectionWriter,
				header.interpolatedPositionJointCount,
				header.usedPositionJointCount,
				header.defaultPosePositionJointCount,
				m_jointPositionKeys,
				m_defaultPosePosition,
				header,
				3u,
				false,
				getPositionFactor(),
				getPositionTangentFactor()
			);

			writeChunkData(
				sectionWriter,
				header.interpolatedScaleJointCount,
				header.usedScaleJointCount,
				header.defaultPoseScaleJointCount,
				m_jointScaleKeys,
				m_defaultPoseScale,
				header,
				3u,
				false,
				getScaleFactor(),
				getScaleTangentFactor()
			);

			sectionWriter.writeAlignment( 8u );
		}
		resourceWriter.closeDataSection( sectionWriter );

		const uint16 headerOffset = sizeof( float ) * 4u + sizeof( uint16 ) * 4u + sizeof( ResRef< void > );
		resourceWriter.openDataSection( sectionWriter, SectionType_Main, 4u );
		targetKey = sectionWriter.addDataPoint();

		sectionWriter.writeFloat( getPositionFactor() );
		sectionWriter.writeFloat( getPositionTangentFactor() );
		sectionWriter.writeFloat( getScaleFactor() );
		sectionWriter.writeFloat( getScaleTangentFactor() );

		sectionWriter.writeUInt16( headerOffset );
		sectionWriter.writeUInt16( (uint16)m_headers.getCount() );

		sectionWriter.writeUInt16( (uint16)(m_lengthInFrames - 1u )  );
		sectionWriter.writeUInt16( (uint16)m_jointCount );

		sectionWriter.writeReference( &animationDataKey );

		// write header
		for (uint i = 0; i < m_headers.getCount(); ++i)
		{
			const ChunkHeader& header = m_headers[ i ];

			sectionWriter.writeAlignment( 4u );
			sectionWriter.writeUInt32( uint32( header.dataOffset ) );

			sectionWriter.writeUInt16( header.startTime );
			sectionWriter.writeUInt16( header.endTime );

			sectionWriter.writeUInt16( header.usedRotationJointCount );
			sectionWriter.writeUInt16( header.interpolatedRotationJointCount );
			sectionWriter.writeUInt16( header.defaultPoseRotationJointCount );

			sectionWriter.writeUInt16( header.usedPositionJointCount );
			sectionWriter.writeUInt16( header.interpolatedPositionJointCount );
			sectionWriter.writeUInt16( header.defaultPosePositionJointCount );

			sectionWriter.writeUInt16( header.usedScaleJointCount );
			sectionWriter.writeUInt16( header.interpolatedScaleJointCount );
			sectionWriter.writeUInt16( header.defaultPoseScaleJointCount );
		}

		resourceWriter.closeDataSection( sectionWriter );
	}

	void AnimationBuilder::writeChunkData( ResourceSectionWriter& sectionWriter, uint16& interpolatedJointCount, uint16& usedJointCount, uint16& defaultPoseJointCount, const Array< List< SplineKey > >& sourceData, const Array< DecomposedValue >& defaultPose, ChunkHeader& header, uint dimensions, bool normalizeValue, float factor, float factorTangent )
	{
		List< uint64 > masks;
		List< sint16 > interpolationData;
		List< sint16 > constantData;
		List< uint16 > defaultPoseJointIndices;

		for (uint jointIndex = 0; jointIndex < m_jointCount; ++jointIndex)
		{
			List< SplineKey > sourceKeys;
			const uint64 mask = generateBitMaskAndFindKeys( sourceKeys, sourceData[ jointIndex ], header.startTime, header.endTime, dimensions, normalizeValue );

			if ( sourceKeys.getCount() == 0 )
			{
				defaultPoseJointIndices.add( uint32( jointIndex ) );
				continue;
			}
			else
			{
				bool isConstJoint = ( sourceKeys.getCount() <= 2u );
				if ( isConstJoint && sourceKeys.getCount() == 2u )
				{
					const SplineKey& key0 = sourceKeys[ 0u ];
					const SplineKey& key1 = sourceKeys[ 1u ];

					for (uint i = 0; i < dimensions; ++i)
					{
						if ( !f32::isEquals( key0.aValues[ i ], key1.aValues[ i ], m_constEpsilon ) )
						{
							isConstJoint = false;
							break;
						}
					}
				}

				if ( isConstJoint )
				{
					bool isEquals = true;
					for (uint i = 0; i < dimensions; ++i)
					{
						if (!f32::isEquals( defaultPose[ jointIndex ].aValues[ i ], sourceKeys[ 0 ].aValues[ i ], m_constEpsilon ) )
						{
							isEquals = false;
							break;
						}
					}

					if ( isEquals )
					{
						defaultPoseJointIndices.add( uint16( jointIndex ) );
						continue;
					}

					constantData.add( sint16( jointIndex ) );
					fillDataArray( constantData, sourceKeys[ 0u ], dimensions, factor, factorTangent, true );
				}
				else
				{
					TIKI_ASSERT( mask > 0ull );

					masks.add(mask);
					interpolationData.add( sint16( jointIndex ) );

					for (uint k = 0; k < sourceKeys.getCount(); ++k)
					{
						fillDataArray( interpolationData, sourceKeys[ k ], dimensions, factor, factorTangent, false );
					}

					interpolatedJointCount++;
				}
			}

			usedJointCount++;
		}

		sectionWriter.writeAlignment( 8u );
		for (uint i = 0u; i < masks.getCount(); ++i)
		{
			sectionWriter.writeUInt64( masks[ i ] );
		}

		for (uint i = 0u; i < interpolationData.getCount(); ++i)
		{
			sectionWriter.writeSInt16( interpolationData[ i ] );
		}

		for (uint i = 0u; i < constantData.getCount(); ++i)
		{
			sectionWriter.writeSInt16( constantData[ i ] );
		}

		for (uint i = 0u; i < defaultPoseJointIndices.getCount(); ++i)
		{
			sectionWriter.writeUInt16( defaultPoseJointIndices[ i ] );
		}

		defaultPoseJointCount = uint16( defaultPoseJointIndices.getCount() );
	}

	void AnimationBuilder::fillDataArray( List< sint16 >& target, const SplineKey& sourceKey, uint dimensions, float factor, float factorTangent, bool isConstKey )
	{
		const sint16 aValues[] =
		{
			(sint16)(sourceKey.aValues[ 0u ] / factor),
			(sint16)(sourceKey.aValues[ 1u ] / factor),
			(sint16)(sourceKey.aValues[ 2u ] / factor),
			(sint16)(sourceKey.aValues[ 3u ] / factor)
		};

		for (uint i = 0; i < dimensions; ++i)
		{
			target.add( aValues[ i ] );
		}

		if ( !isConstKey )
		{
			const sint16 aControls[] =
			{
				(sint16)(sourceKey.aControls[ 0u ] / factorTangent),
				(sint16)(sourceKey.aControls[ 1u ] / factorTangent),
				(sint16)(sourceKey.aControls[ 2u ] / factorTangent),
				(sint16)(sourceKey.aControls[ 3u ] / factorTangent)
			};

			for (uint i = 0; i < dimensions; ++i)
			{
				target.add( aControls[ i ] );
			}
		}
	}

	SplineKey AnimationBuilder::createInterpolatedKey( const SplineKey& leftKey, const SplineKey& rightKey, uint time, uint dimensions, bool normalizeValue )
	{
		TIKI_ASSERT( dimensions <= SplineLimits_MaxDimensions );

		const float koeff = float(time - leftKey.time) / float(rightKey.time - leftKey.time);

		SplineKey key;
		key.time = (uint16)time;
		SplineKey::sampleHermite( key.aValues, dimensions, leftKey, rightKey, koeff );

		for (uint i = 0; i < dimensions; ++i)
		{
			key.aControls[ i ] = f32::lerp( leftKey.aControls[ i ], rightKey.aControls[ i ], koeff );
		}

		if ( normalizeValue )
		{
			float lengthSquad = 0.0f;
			for (uint i = 0; i < dimensions; ++i)
			{
				lengthSquad += key.aValues[ i ] * key.aValues[ i ];
			}
			const float length = 1.0f / (float)sqrt( lengthSquad );

			for (uint i = 0; i < dimensions; ++i)
			{
				key.aValues[ i ] *= length;
			}
		}

		return key;
	}

	uint64 AnimationBuilder::generateBitMaskAndFindKeys( List< SplineKey >& targetList, const List< SplineKey >& sourceKeys, uint16 startTime, uint16 endTime, uint dimensions, bool normalizeValue )
	{
		if (sourceKeys.getCount() == 0u)
		{
			return 0u;
		}
		else if (sourceKeys.getCount() == 1u)
		{
			targetList.add( sourceKeys[ 0u ] );
			return 0u;
		}
		else if (sourceKeys.getCount() == 2u)
		{
			const float* f1 = sourceKeys[ 0 ].aValues;
			const float* f2 = sourceKeys[ 1 ].aValues;

			bool equals = true;
			for (uint i = 0; i < dimensions; ++i)
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

		uint firstIndex	= TIKI_SIZE_T_MAX;
		uint lastIndex	= TIKI_SIZE_T_MAX;
		SplineKey firstKey;
		SplineKey lastKey;
		for (uint i = 0; i < sourceKeys.getCount(); i++)
		{
			if ( firstIndex == TIKI_SIZE_T_MAX )
			{
				if ( sourceKeys[ i ].time > startTime )
				{
					firstIndex = i - 1u;
				}
				else if ( sourceKeys[ i ].time == startTime )
				{
					firstIndex = i;
				}
			}

			if ( sourceKeys[ i ].time >= endTime || i == sourceKeys.getCount() - 1u )
			{
				lastIndex = i;
				break;
			}
		}
		TIKI_ASSERT( firstIndex != TIKI_SIZE_T_MAX );

		if ( sourceKeys[ firstIndex ].time != startTime )
		{
			firstKey = createInterpolatedKey( sourceKeys[ firstIndex ], sourceKeys[ firstIndex + 1 ], startTime, dimensions, normalizeValue );
		}
		else
		{
			firstKey = sourceKeys[ firstIndex ];
		}
		firstIndex++;

		SplineKey sourceLastKey = sourceKeys[ lastIndex ];
		if ( sourceKeys[ lastIndex ].time != endTime )
		{
			lastKey = createInterpolatedKey( sourceKeys[ lastIndex - 1 ], sourceKeys[ lastIndex ], endTime, dimensions, normalizeValue );
		}
		else
		{
			lastKey = sourceKeys[ lastIndex ];
		}

		uint64 mask = 0;

		insertBitMask( targetList, mask, firstKey, startTime );
		for (uint i = firstIndex; i < lastIndex; i++)
		{
			insertBitMask( targetList, mask, sourceKeys[ i ], startTime );
		}

		if (sourceLastKey.time <= endTime && sourceLastKey.time != startTime)
		{
			insertBitMask( targetList, mask, sourceLastKey, startTime );
		}

		if (lastKey.time < sourceLastKey.time)
		{
			insertBitMask( targetList, mask, lastKey, startTime );
		}

		if ( !isBitSet64( mask, 0x8000000000000001ull ) && sourceLastKey.time >= endTime )
		{
			TIKI_TRACE_ERROR( "incomplete bit mask in chunk: %i", (startTime / (endTime - startTime)) );
		}

		return mask;
	}

	void AnimationBuilder::insertBitMask( List< SplineKey >& targetList, uint64& mask, const SplineKey& key, uint16 startTime )
	{
		const uint64 orMask = uint64( 1ull ) << uint64( key.time - startTime );
		if ( isBitSet64( mask, orMask ) )
		{
			TIKI_TRACE_ERROR( "bit mask overlapping. this should not happen.\n" );
		}

		mask |= orMask;
		targetList.add( key );
	}
}
