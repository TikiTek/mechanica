
#include "tiki/animation/animation.hpp"

#include "tiki/animation/animationjoint.hpp"
#include "tiki/base/simd.hpp"
#include "tiki/graphics/modelhierarchy.hpp"
#include "tiki/resource/resourcemanager.hpp"

namespace tiki
{
	struct AnimationFactoryContext : public FactoryContextGenericBase< Animation >
	{
		AnimationFactoryContext()
		{
		}
	};

	void Animation::registerResourceType( ResourceManager& resourceManager )
	{
		static AnimationFactoryContext context;
		resourceManager.registerResourceType( s_resourceType, context );
	}

	void Animation::unregisterResourceType( ResourceManager& resourceManager )
	{
		resourceManager.unregisterResourceType( s_resourceType );
	}

	struct AnimationInitData
	{
		ResRef< AnimationData > data;
	};

	Animation::Animation()
	{
		m_pData = nullptr;
	}

	Animation::~Animation()
	{
		TIKI_ASSERT( m_pData == nullptr );
	}

	bool Animation::createInternal( const ResourceInitData& initData, const FactoryContext& /*factoryContext*/ )
	{
		const AnimationInitData& animationInitData = *static_cast< const AnimationInitData* >( initData.pData );

		m_pData = animationInitData.data.getData();
		return m_pData != nullptr;
	}

	void Animation::disposeInternal( const FactoryContext& /*factoryContext*/ )
	{
		m_pData = nullptr;
	}

	static TIKI_FORCE_INLINE uint getCountLeftBit( uint* pLeftBits, uint64 mask, uint bit )
	{
		const uint64 andMask = mask & ((uint64)-1 >> (64 - bit));
		*pLeftBits = 63u - countLeadingZeros64( andMask );

		return (uint)countPopulation64( andMask ) - 1u;
	}

	static TIKI_FORCE_INLINE uint getCountRightBit( uint* pRightBits, uint64 mask, uint bit )
	{		
		const uint64 andMask = mask & ((uint64)-1 << bit);
		*pRightBits = 63u - countLeadingZeros64( andMask & (-(sint64)andMask) );

		return (uint)countPopulation64( andMask ) + 1u;
	}

	static TIKI_FORCE_INLINE void decompressRotation( Quaternion& target, const sint16* pData, const vf32 factor )
	{
		const vf32 values = simd::convert_i32_to_f32( simd::convert_i16l_to_i32( simd::set_i16u( pData ) ) );
		const vf32 result = simd::mul_f32( values, factor );

		simd::get_f32( &target.x, result );
	}

	static TIKI_FORCE_INLINE void decompressPosition( Vector3& target, const sint16* pData, const vf32 factor )
	{
		const vf32 values = simd::convert_i32_to_f32( simd::convert_i16l_to_i32( simd::set_i16u( pData ) ) );
		const vf32 result = simd::mul_f32( values, factor );

		simd::get_f32( &target.x, result );
	}

	static TIKI_FORCE_INLINE void decompressScale( Vector3& target, const sint16* pData, const vf32 factor )
	{
		const vf32 values = simd::convert_i32_to_f32( simd::convert_i16l_to_i32( simd::set_i16u( pData ) ) );
		const vf32 result = simd::mul_f32( values, factor );

		simd::get_f32( &target.x, result );
	}

	enum KeySize
	{
		KeySize_Rotation		= 8u,
		KeySize_ConstRotation	= 4u,

		KeySize_Position		= 6u,
		KeySize_ConstPosition	= 3u,

		KeySize_Scale			= 6u,
		KeySize_ConstScale		= 3u
	};

	void Animation::sample( AnimationJoint* pTargetJoints, uint jointCount, const ModelHierarchy& hierarchy, float time ) const
	{
		TIKI_ASSERT( pTargetJoints != nullptr );

		const float fullFrame	= time * 60.0f;
		const uint fullExFrame	= (uint)( fullFrame );
		const float frameDiff	= fullFrame - (float)fullExFrame;
		const uint exactFrame	= fullExFrame % m_pData->frameCount;
		const float frame		= (float)exactFrame + frameDiff;

		uint headerIndex = 0;
		const AnimationChunkHeader* pHeader = addPtrCast< AnimationChunkHeader >( m_pData, m_pData->headerOffset );
		while ( pHeader[ headerIndex ].endTime <= exactFrame )
		{
			TIKI_ASSERT( headerIndex < m_pData->headerCount );
			headerIndex++;
		}
		pHeader += headerIndex;

		const uint localTime		= TIKI_MIN( (exactFrame - pHeader->startTime) + 1u, TIKI_MIN( 63u, m_pData->frameCount - 1u ) );

		const vf32 simdFrame	= simd::set_f32( frame );
		const vf32 all1			= simd::set_f32( 1.0f );
		const vf32 all2			= simd::set_f32( 2.0f );
		const vf32 all3			= simd::set_f32( 3.0f );
		const vf32 allneg2		= simd::set_f32( -2.0f );

		const vf32 rotationFactor			= simd::set_f32( 3.0518513e-005f );
		const vf32 rotationTangentFactor	= simd::set_f32( 7.6293945e-006f );
		const vf32 positionFactor			= simd::set_f32( m_pData->positionFactor );
		const vf32 positionTangentFactor	= simd::set_f32( m_pData->positionTangentFactor );
		const vf32 scaleFactor				= simd::set_f32( m_pData->scaleFactor );
		const vf32 scaleTangentFactor		= simd::set_f32( m_pData->scaleTangentFactor );

		//////////////////////////////////////////////////////////////////////////
		// rotation
		const uint8* pBaseData	= m_pData->data.getData() + pHeader->dataOffset;
		const sint16* pData		= (sint16*)(pBaseData + (sizeof(uint64) * pHeader->interpolatedRotationJointCount));
		const uint64* pMask		= (uint64*)pBaseData;

		{
			uint io = 0;
			for (; io < pHeader->interpolatedRotationJointCount; ++io)
			{
				const uint jointIndex = (uint)pData[ 0u ];
				TIKI_ASSERT( jointIndex < jointCount );
				pData++;

				uint keyIndexLeft = 0;
				uint keyIndexRight = 0;
				const uint keyLeftCount = getCountLeftBit( &keyIndexLeft, *pMask, localTime );
				const uint keyRightCount = getCountRightBit( &keyIndexRight, *pMask, localTime );
				TIKI_ASSERT( keyLeftCount < 64u || keyRightCount < 64u );
				TIKI_ASSERT( keyIndexLeft < 64u || keyIndexRight < 64u );

				const sint32 time1 = (sint32)(pHeader->startTime + keyIndexLeft);
				const sint32 time2 = (sint32)(pHeader->startTime + keyIndexRight);

				pData += keyLeftCount * KeySize_Rotation;

				const vf32 simdTime1	= simd::convert_i32_to_f32( simd::set_i32( time1 ) );
				const vf32 simdTime2	= simd::convert_i32_to_f32( simd::set_i32( time2 ) );
				const vf32 simdTimeT	= simd::div_f32( simd::sub_f32( simdFrame, simdTime1 ), simd::sub_f32( simdTime2, simdTime1 ) );
				const vf32 simdTimeTT	= simd::mul_f32( simdTimeT, simdTimeT );
				const vf32 simdTimeTTT	= simd::mul_f32( simdTimeTT, simdTimeT );
				const vf32 simdScale	= simd::sub_f32( simdTime2, simdTime1 );

				const vi16 sourcePT0 = simd::set_i16u( pData );
				const vi16 sourcePT1 = simd::set_i16u( pData + KeySize_Rotation );

				const vi32 sourceP0 = simd::convert_i16l_to_i32( sourcePT0 );
				const vi32 sourceT0 = simd::convert_i16h_to_i32( sourcePT0 );
				const vi32 sourceP1 = simd::convert_i16l_to_i32( sourcePT1 );
				const vi32 sourceT1 = simd::convert_i16h_to_i32( sourcePT1 );

				const vf32 simdP0 = simd::mul_f32( simd::convert_i32_to_f32( sourceP0 ), rotationFactor );
				vf32 simdP1 = simd::mul_f32( simd::convert_i32_to_f32( sourceP1 ), rotationFactor );

				const vf32 simdDot = simd::dot4_f32( simdP0, simdP1 );
				if ( simd::get_f32_x( simdDot ) < 0.0f )
				{
					simdP1 = simd::sub_f32( simd::set_f32( 0.0f ), simdP1 );
				}

				const vf32 simdT0 = simd::mul_f32( simd::mul_f32( simd::convert_i32_to_f32( sourceT0 ), rotationTangentFactor ), simdScale );
				const vf32 simdT1 = simd::mul_f32( simd::mul_f32( simd::convert_i32_to_f32( sourceT1 ), rotationTangentFactor ), simdScale );

				const vf32 simdResult1 = simd::muladd_f32( simd::add_f32( simd::mulsub_f32( all2, simdTimeTTT, simd::mul_f32( all3, simdTimeTT ) ), all1 ), simdP0, simd::muladd_f32( simd::add_f32( simd::negmulsub_f32( all2, simdTimeTT, simdTimeTTT ), simdTimeT ), simdT0, simd::muladd_f32( simd::muladd_f32( allneg2, simdTimeTTT, simd::mul_f32( all3, simdTimeTT ) ), simdP1, simd::mul_f32( simd::sub_f32( simdTimeTTT, simdTimeTT ), simdT1 ) ) ) );
				const vf32 simdResult2 = simd::mul_f32( simdResult1, simd::rsqrt_f32( simd::dot4_f32( simdResult1, simdResult1 ) ) );

				simd::get_f32( &pTargetJoints[ jointIndex ].rotation.x, simdResult2 );

				pData += keyRightCount * KeySize_Rotation;
				pMask++;
			}

			// const rotation
			for (; io < pHeader->usedRotationJointCount; ++io)
			{
				const uint jointIndex = (uint)pData[ 0u ];
				TIKI_ASSERT( jointIndex < jointCount );
				pData++;

				decompressRotation( pTargetJoints[ jointIndex ].rotation, pData, rotationFactor );
				pData += KeySize_ConstRotation;
			}

			// unused rotation
			const uint16* pUnused = (uint16*)pData;				
			const vf32* pDefaultPoseRotation = hierarchy.getDefaultPoseRotation();
			for (uint i = 0; i < pHeader->defaultPoseRotationJointCount; ++i)
			{
				const uint jointIndex = pUnused[ 0u ];
				TIKI_ASSERT( jointIndex < jointCount );
				pUnused++;

				simd::get_f32( &pTargetJoints[ jointIndex ].rotation.x, pDefaultPoseRotation[ jointIndex ] );
			}

			pData = (sint16*)pUnused;
		}


		//////////////////////////////////////////////////////////////////////////
		// position
		pBaseData = (uint8*)alignPtr( pData, 8u );
		pData = (sint16*)(pBaseData + (sizeof(uint64) * pHeader->interpolatedPositionJointCount));
		pMask = (uint64*)pBaseData;

		{
			uint ip = 0;
			for (; ip < pHeader->interpolatedPositionJointCount; ++ip)
			{
				const uint jointIndex = (uint)pData[ 0u ];
				TIKI_ASSERT( jointIndex < jointCount );
				pData++;

				uint keyIndexLeft = 0;
				uint keyIndexRight = 0;
				const uint keyLeftCount = getCountLeftBit( &keyIndexLeft, *pMask, localTime );
				const uint keyRightCount = getCountRightBit( &keyIndexRight, *pMask, localTime );
				TIKI_ASSERT( keyLeftCount < 64u || keyRightCount < 64u );
				TIKI_ASSERT( keyIndexLeft < 64u || keyIndexRight < 64u );

				const sint32 time1 = (sint32)(pHeader->startTime + keyIndexLeft);
				const sint32 time2 = (sint32)(pHeader->startTime + keyIndexRight);

				pData += keyLeftCount * KeySize_Position;

				const vf32 simdTime1	= simd::convert_i32_to_f32( simd::set_i32( time1 ) );
				const vf32 simdTime2	= simd::convert_i32_to_f32( simd::set_i32( time2 ) );
				const vf32 simdTimeT	= simd::div_f32( simd::sub_f32( simdFrame, simdTime1 ), simd::sub_f32( simdTime2, simdTime1 ) );
				const vf32 simdTimeTT	= simd::mul_f32( simdTimeT, simdTimeT );
				const vf32 simdTimeTTT	= simd::mul_f32( simdTimeTT, simdTimeT );
				const vf32 simdScale	= simd::sub_f32( simdTime2, simdTime1 );

				const vi32 sourceP0 = simd::convert_i16l_to_i32( simd::set_i16u( &pData[ 0u ] ) );
				const vi32 sourceT0 = simd::convert_i16l_to_i32( simd::set_i16u( &pData[ 3u ] ) );
				const vi32 sourceP1 = simd::convert_i16l_to_i32( simd::set_i16u( &pData[ 6u ] ) );
				const vi32 sourceT1 = simd::convert_i16l_to_i32( simd::set_i16u( &pData[ 9u ] ) );

				const vf32 simdP0 = simd::mul_f32( simd::convert_i32_to_f32( sourceP0 ), positionFactor );
				const vf32 simdP1 = simd::mul_f32( simd::convert_i32_to_f32( sourceP1 ), positionFactor );

				const vf32 simdT0 = simd::mul_f32( simd::mul_f32( simd::convert_i32_to_f32( sourceT0 ), positionTangentFactor ), simdScale );
				const vf32 simdT1 = simd::mul_f32( simd::mul_f32( simd::convert_i32_to_f32( sourceT1 ), positionTangentFactor ), simdScale );

				const vf32 simdResult = simd::muladd_f32( simd::add_f32( simd::mulsub_f32( all2, simdTimeTTT, simd::mul_f32( all3, simdTimeTT ) ), all1 ), simdP0, simd::muladd_f32( simd::add_f32( simd::negmulsub_f32( all2, simdTimeTT, simdTimeTTT ), simdTimeT ), simdT0, simd::muladd_f32( simd::muladd_f32( allneg2, simdTimeTTT, simd::mul_f32( all3, simdTimeTT ) ), simdP1, simd::mul_f32( simd::sub_f32( simdTimeTTT, simdTimeTT ), simdT1 ) ) ) );

				simd::get_f32( &pTargetJoints[ jointIndex ].position.x, simdResult );

				pData += keyRightCount * KeySize_Position;
				pMask++;
			}

			// const position
			for (; ip < pHeader->usedPositionJointCount; ++ip)
			{
				const uint jointIndex = (uint)pData[ 0u ];
				TIKI_ASSERT( jointIndex < jointCount );
				pData++;

				decompressPosition( pTargetJoints[ jointIndex ].position, pData, positionFactor );
				pData += KeySize_ConstPosition;
			}

			// unused position
			const uint16* pUnused = (uint16*)pData;				
			const vf32* pDefaultPosePosition = hierarchy.getDefaultPosePosition();
			for (uint i = 0; i < pHeader->defaultPosePositionJointCount; ++i)
			{
				const uint jointIndex = pUnused[ 0u ];
				TIKI_ASSERT( jointIndex < jointCount );
				pUnused++;

				simd::get_f32( &pTargetJoints[ jointIndex ].position.x, pDefaultPosePosition[ jointIndex ] );
			}

			pData = (sint16*)pUnused;
		}

		//////////////////////////////////////////////////////////////////////////
		// scale
		//////////////////////////////////////////////////////////////////////////
		pBaseData = (uint8*)alignPtr( pData, 8u );
		pData = (sint16*)(pBaseData + (sizeof(uint64) * pHeader->interpolatedScaleJointCount));
		pMask = (uint64*)pBaseData;

		{
			uint is = 0;
			for (; is < pHeader->interpolatedScaleJointCount; ++is)
			{
				const uint jointIndex = (uint)pData[ 0u ];
				TIKI_ASSERT( jointIndex < jointCount );
				pData++;

				uint keyIndexLeft = 0;
				uint keyIndexRight = 0;
				const uint keyLeftCount = getCountLeftBit( &keyIndexLeft, *pMask, localTime );
				const uint keyRightCount = getCountRightBit( &keyIndexRight, *pMask, localTime );
				TIKI_ASSERT( keyLeftCount < 64u || keyRightCount < 64u );
				TIKI_ASSERT( keyIndexLeft < 64u || keyIndexRight < 64u );

				const sint32 time1 = (sint32)(pHeader->startTime + keyIndexLeft);
				const sint32 time2 = (sint32)(pHeader->startTime + keyIndexRight);

				pData += keyLeftCount * KeySize_Scale;

				const vf32 simdTime1	= simd::convert_i32_to_f32( simd::set_i32( time1 ) );
				const vf32 simdTime2	= simd::convert_i32_to_f32( simd::set_i32( time2 ) );
				const vf32 simdTimeT	= simd::div_f32( simd::sub_f32( simdFrame, simdTime1 ), simd::sub_f32( simdTime2, simdTime1 ) );
				const vf32 simdTimeTT	= simd::mul_f32( simdTimeT, simdTimeT );
				const vf32 simdTimeTTT	= simd::mul_f32( simdTimeTT, simdTimeT );
				const vf32 simdScale	= simd::sub_f32( simdTime2, simdTime1 );

				const vi32 sourceP0 = simd::convert_i16l_to_i32( simd::set_i16u( &pData[ 0u ] ) );
				const vi32 sourceT0 = simd::convert_i16l_to_i32( simd::set_i16u( &pData[ 3u ] ) );
				const vi32 sourceP1 = simd::convert_i16l_to_i32( simd::set_i16u( &pData[ 6u ] ) );
				const vi32 sourceT1 = simd::convert_i16l_to_i32( simd::set_i16u( &pData[ 9u ] ) );

				const vf32 simdP0 = simd::mul_f32( simd::convert_i32_to_f32( sourceP0 ), scaleFactor );
				const vf32 simdP1 = simd::mul_f32( simd::convert_i32_to_f32( sourceP1 ), scaleFactor );

				const vf32 simdT0 = simd::mul_f32( simd::mul_f32( simd::convert_i32_to_f32( sourceT0 ), scaleTangentFactor ), simdScale );
				const vf32 simdT1 = simd::mul_f32( simd::mul_f32( simd::convert_i32_to_f32( sourceT1 ), scaleTangentFactor ), simdScale );

				const vf32 simdResult = simd::muladd_f32( simd::add_f32( simd::mulsub_f32( all2, simdTimeTTT, simd::mul_f32( all3, simdTimeTT ) ), all1 ), simdP0, simd::muladd_f32( simd::add_f32( simd::negmulsub_f32( all2, simdTimeTT, simdTimeTTT ), simdTimeT ), simdT0, simd::muladd_f32( simd::muladd_f32( allneg2, simdTimeTTT, simd::mul_f32( all3, simdTimeTT ) ), simdP1, simd::mul_f32( simd::sub_f32( simdTimeTTT, simdTimeTT ), simdT1 ) ) ) );

				simd::get_f32( &pTargetJoints[ jointIndex ].scale.x, simdResult );

				pData += keyRightCount * KeySize_Scale;
				pMask++;
			}

			// const scale
			for (; is < pHeader->usedScaleJointCount; ++is)
			{
				const uint jointIndex = (uint)pData[ 0u ];
				TIKI_ASSERT( jointIndex < jointCount );
				pData++;

				decompressScale( pTargetJoints[ jointIndex ].scale, pData, scaleFactor );
				pData += KeySize_ConstScale;
			}

			// unused scale
			const uint16* pUnused = (uint16*)pData;				
			const vf32* pDefaultPoseScale = hierarchy.getDefaultPoseScale();
			for (uint i = 0; i < pHeader->defaultPoseScaleJointCount; ++i)
			{
				const uint jointIndex = pUnused[ 0u ];
				TIKI_ASSERT( jointIndex < jointCount );
				pUnused++;

				simd::get_f32( &pTargetJoints[ jointIndex ].scale.x, pDefaultPoseScale[ jointIndex ] );
			}

			pData = (sint16*)pUnused;
		}
	}
}
