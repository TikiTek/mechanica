
#include "tiki/animationconverter/spline.hpp"

#include "tiki/container/list.hpp"

#include <math.h>

namespace tiki
{
	void SplineKey::sampleHermite( float* pTarget, uint targetCapacity, const SplineKey& leftKey, const SplineKey& rightKey, float t )
	{
		const float tangentScale = (float)( rightKey.time - leftKey.time );

		const float tt	= t * t;
		const float ttt	= tt * t;
		for (uint i = 0; i < targetCapacity; ++i)
		{
			const float p0 = leftKey.aValues[ i ];
			const float p1 = rightKey.aValues[ i ];
			const float t0 = leftKey.aControls[ i ] * tangentScale;
			const float t1 = rightKey.aControls[ i ] * tangentScale;

			pTarget[ i ] = (2.0f * p0 - 2.0f * p1 + t1 + t0) * ttt + (3.0f * p1 - 3.0f * p0 - 2.0f * t0 - t1) * tt + t0 * t + p0;
		}
	}

	Spline::Spline()
	{
		m_lengthImFrames	= 0u;
		m_dimensions		= 0u;
	}

	Spline::~Spline()
	{
	}

	void Spline::createFromKeys( const SplineKey* pSourceKeys, uint keyCount, uint dimensions, uint lengthInFrames )
	{
		TIKI_ASSERT( pSourceKeys != nullptr );
		TIKI_ASSERT( dimensions <= SplineLimits_MaxDimensions );

		m_lengthImFrames	= lengthInFrames;
		m_dimensions		= dimensions;

		m_keys = List< SplineKey >( pSourceKeys, keyCount );
	}

	void Spline::createFromSamples( const float* pSource, uint dimensions, uint lengthInFrames, float threshold )
	{
		TIKI_ASSERT( dimensions <= SplineLimits_MaxDimensions );

		m_lengthImFrames	= lengthInFrames;
		m_dimensions		= dimensions;

		SplineKey firstKey = createKey( pSource, 0u, dimensions );
		m_keys.add( firstKey );

		if ( lengthInFrames == 1u )
		{
			return;
		}

		SplineKey lastKey = createKey( pSource, lengthInFrames - 1u, dimensions );
		m_keys.add( lastKey );

		uint count = 0u;
		uint lenCount = lengthInFrames / 2u;
		float maxError = 0.0f;
		do
		{
			maxError = 0.0f;

			uint errorTime = 0u;
			float errorError = 0.0f;
			for (uint i = 1u; i < m_keys.getCount(); ++i)
			{
				const SplineKey& leftKey = m_keys[ i - 1u ];
				const SplineKey& rightKey = m_keys[ i ];

				for (uint time = leftKey.time; time < rightKey.time; ++time)
				{
					float currentValue[ SplineLimits_MaxDimensions ];
					const float koeff = ((float)time - leftKey.time) / (rightKey.time - leftKey.time);
					SplineKey::sampleHermite( currentValue, dimensions, leftKey, rightKey, koeff );

					float localError = 0.0f;
					const float* pSourceValue = pSource + time * dimensions;
					for (uint j = 0; j < dimensions; ++j)
					{
						const float dimError = fabsf( currentValue[ j ] - pSourceValue[ j ] );
						localError = TIKI_MAX(localError, dimError);
					}

					if ( localError > errorError )
					{
						errorTime = time;
						errorError = localError;
						maxError = TIKI_MAX( maxError, localError );
					}
				}
			}

			if ( errorTime != 0u && errorError >= threshold )
			{
				insertKey( createKey( pSource, errorTime, dimensions ) );
			}

			calculateTangents( pSource, lengthInFrames, dimensions );

			count++;
		}
		while ( maxError > threshold && count < lenCount );
		TIKI_ASSERT( m_keys.getLast().time == lengthInFrames - 1u );
	}

	void Spline::dispose()
	{
		m_keys.dispose();
	}

	void Spline::sample( float* pTarget, uint targetCapacity, float sampleTime ) const
	{
		TIKI_ASSERT( pTarget != nullptr );
		TIKI_ASSERT( m_keys.getCount() != 0u );
		TIKI_ASSERT( targetCapacity <= m_dimensions );

		const float frame		= 60u * sampleTime;
		const uint32 exactFrame	= (uint32)frame;
		TIKI_ASSERT( m_keys.getLast().time >= exactFrame );

		if ( m_keys.getCount() == 1u )
		{
			for (uint i = 0; i < targetCapacity; ++i)
			{
				pTarget[ i ] = m_keys[ 0u ].aValues[ i ];
			}
			return;
		}

		const SplineKey* pLeftKey	= &m_keys[ 0u ];
		const SplineKey* pRightKey	= &m_keys[ 1u ];

		while ( pRightKey->time < exactFrame )
		{
			pLeftKey++;
			pRightKey++;
		}

		const float time = ( frame - pLeftKey->time ) / ( pRightKey->time - pLeftKey->time );
		SplineKey::sampleHermite( pTarget, targetCapacity, *pLeftKey, *pRightKey, time );
	}

	void Spline::insertKey( const SplineKey& key )
	{
		for (uint i = 0; i < m_keys.getCount(); ++i)
		{
			const SplineKey& currentKey = m_keys[ i ];
			if (currentKey.time == key.time)
			{
				// already exists
				return;
			}

			if (currentKey.time > key.time)
			{
				m_keys.insert( i, key );
				return;
			}
		}
	}

	SplineKey Spline::createKey( const float* pSource, uint time, uint dimensions )
	{
		const float* pSourceKey = pSource + ( time * dimensions );

		SplineKey key;
		key.time = (uint32)time;

		for (uint i = 0; i < dimensions; ++i)
		{
			key.aValues[ i ] = pSourceKey[ i ];
			key.aControls[ i ] = 0.0f;
		}

		return key;
	}

	void Spline::calculateTangents( const float* pSource, uint sourceLength, uint dimensions )
	{
		const float t = 0.5f;

		for (uint i = 0; i < m_keys.getCount(); ++i)
		{
			SplineKey& key1			= m_keys[ i ];
			const SplineKey& key2	= m_keys[ i == m_keys.getCount() - 1u ? i : i + 1u ];

			const float* pLeft		= pSource + (key1.time == sourceLength - 1u ? key1.time - 1u : key1.time ) * dimensions;
			const float* pRight		= pSource + (key1.time == sourceLength - 1u ? key1.time : key1.time + 1u ) * dimensions;
			const float* pRightPlus	= pSource + clamp( key2.time + 1u, 0u, sourceLength - 1u ) * dimensions;
			for (uint dim = 0; dim < dimensions; ++dim)
			{
				const float y2my1 = (pRight[ dim ] - pLeft[ dim ]);
				const float x2mx1 = 0.5f;

				const float a = (pLeft[ dim ] - key1.aValues[ dim ]) * x2mx1 - y2my1;
				const float b = -(key2.aValues[ dim ] - pRightPlus[ dim ]) * x2mx1 - y2my1;

				const float q1 = (y2my1 / x2mx1) + (1.0f - 2.0f * t) * ((a * (1.0f - t) + b * t) / x2mx1) + t * (1.0f - t) * ((b - a) / x2mx1);
				const float tangent = q1 / 2.0f;

				key1.aControls[ dim ] = tangent;
			}
		}
	}
}
