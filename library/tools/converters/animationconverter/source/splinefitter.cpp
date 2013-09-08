
#include "tiki/animationconverter/splinefitter.hpp"

#include "tiki/toolbase/list.hpp"

namespace tiki
{
	static void splineInsertKey( List< SplineKey >& target, const SplineKey& key )
	{
		for (size_t i = 0; i < target.getCount(); ++i)
		{
			const SplineKey& currentKey = target[ i ];

			if (currentKey.time == key.time)
			{
				return;
			}

			if (currentKey.time > key.time)
			{
				target.insert( i, key );
				return;
			}
		}
	}

	SplineKey splineCreateKey( const float* pSource, size_t time, size_t dimentions )
	{
		const float* pSourceKey = pSource + time * dimentions;

		SplineKey key;
		key.time = (uint32)time;

		for (size_t i = 0; i < dimentions; ++i)
		{
			key.values[ i ] = pSourceKey[ i ];
			key.controls[ i ] = 0.0f;
		}

		return key;
	}

	void splineCalculateTangents( List< SplineKey >& target, const float* pSource, size_t sourceLength, size_t dimentions )
	{
		for (size_t i = 0; i < target.getCount(); ++i)
		{
			SplineKey& key1 = target[ i ];
			const SplineKey& key2 = target[ i == target.getCount() - 1u ? i : i + 1u ];

			const float* pLeft = pSource + (key1.time == sourceLength - 1u ? key1.time - 1u : key1.time ) * dimentions;
			const float* pRight = pSource + (key1.time == sourceLength - 1u ? key1.time : key1.time + 1u ) * dimentions;
			const float* pRightPlus = pSource + clamp(key2.time + 1u, 0u, (uint32)sourceLength - 1u) * dimentions;

			for (size_t dim = 0; dim < dimentions; ++dim)
			{
				const float t = 0.5f;

				const float y2my1 = (pRight[ dim ] - pLeft[ dim ]);
				const float x2mx1 = 0.5f;

				const float a = (pLeft[ dim ] - key1.values[ dim ]) * x2mx1 - y2my1;
				const float b = -(key2.values[ dim ] - pRightPlus[ dim ]) * x2mx1 - y2my1;

				const float q1 = (y2my1 / x2mx1) + (1.0f - 2.0f * t) * ((a * (1.0f - t) + b * t) / x2mx1) + t * (1.0f - t) * ((b - a) / x2mx1);
				const float tangent = q1 / 2.0f;

				key1.controls[ dim ] = tangent;
			}
		}
	}

	float splineCreateKeys( List< SplineKey >& target, const float* pSource, size_t sourceLength, size_t dimentions, float threshold )
	{		
		SplineKey firstKey = splineCreateKey( pSource, 0u, dimentions );
		target.add( firstKey );

		if ( sourceLength == 1u )
		{
			return 0.0f;
		}

		SplineKey lastKey = splineCreateKey( pSource, sourceLength - 1u, dimentions );
		target.add( lastKey );

		size_t count = 0u;
		size_t lenCount = sourceLength / 2;
		float maxError = 0.0f;
		do 
		{
			maxError = 0.0f;

			size_t errorTime = 0u;
			float errorError = 0.0f;

			for (size_t i = 1u; i < target.getCount(); ++i)
			{
				const SplineKey& leftKey = target[ i - 1u ];
				const SplineKey& rightKey = target[ i ];

				for (size_t time = leftKey.time; time < rightKey.time; ++time)
				{
					float currentValue[ 4u ];
					const float koeff = ((float)time - leftKey.time) / (rightKey.time - leftKey.time);
					sampleSpline( currentValue, dimentions, leftKey.values, rightKey.values, leftKey.controls, rightKey.controls, (float)(rightKey.time - leftKey.time), koeff );

					const float* pSourceValue = pSource + time * dimentions;

					float localError = 0.0f;
					for (size_t j = 0; j < dimentions; ++j)
					{
						const float dimError = abs( currentValue[ j ] - pSourceValue[ j ] );
						localError = TIKI_MAX(localError, dimError);
					}

					if (localError > errorError)
					{
						errorTime = time;
						errorError = localError;
						maxError = TIKI_MAX( maxError, localError );
					}
				}
			}

			if (errorTime != 0u && errorError >= threshold)
			{
				splineInsertKey(
					target,
					splineCreateKey( pSource, errorTime, dimentions )
				);
			}

			splineCalculateTangents( target, pSource, sourceLength, dimentions );

			count++;
		}
		while ( maxError > threshold && count < lenCount );

		return maxError;
	}

	void createSpline( Spline* pSpline, size_t dimensions, size_t length, const SplineKey* pKeys, size_t keyCount )
	{
		TIKI_ASSERT( pSpline );

		pSpline->dimensions = dimensions;
		pSpline->length = length;

		pSpline->keyData.create( keyCount );
		
		for (size_t i = 0; i < keyCount; ++i)
		{
			pSpline->keyData[ i ] = pKeys[ i ];
		}
	}

	void destroySpline( Spline* pSpline )
	{
		pSpline->keyData.dispose();
	}

	void sampleSpline( float* pTarget, const Spline* pSpline, float time )
	{
		TIKI_ASSERT( pSpline->keyData.getCount() > 0u );
		
		const float frame = 60u * time;
		const uint32 exactFrame = (uint32)frame;
		TIKI_ASSERT( pSpline->keyData[ pSpline->keyData.getCount() - 1u ].time >= exactFrame );

		if ( pSpline->keyData.getCount() == 1u )
		{
			for (size_t i = 0; i < pSpline->dimensions; ++i)
			{
				pTarget[ i ] = pSpline->keyData[ 0u ].values[ i ];
			}
			return;
		}

		const SplineKey* pLeftKey	= &pSpline->keyData[ 0u ];
		const SplineKey* pRightKey	= &pSpline->keyData[ 1u ];

		while ( pRightKey->time < exactFrame )
		{
			pLeftKey++;
			pRightKey++;
		}

		const float tscale = (float)( pRightKey->time - pLeftKey->time );
		const float koeff = ( frame - pLeftKey->time ) / tscale;
		sampleSpline( pTarget, pSpline->dimensions, pLeftKey->values, pRightKey->values, pLeftKey->controls, pRightKey->controls, tscale, koeff );
	}

	void sampleSpline( float* pTarget, size_t dimensions, const float* pPoint0, const float* pPoint1, const float* pTangent0, const float* pTangent1, float tangentScale, float time )
	{
		const float t = time;
		const float tt = time * time;
		const float ttt = time * time * time;

		for (size_t i = 0; i < dimensions; ++i)
		{
			const float p0 = pPoint0[ i ];
			const float p1 = pPoint1[ i ];
			const float t0 = pTangent0[ i ] * tangentScale;
			const float t1 = pTangent1[ i ] * tangentScale;

			pTarget[ i ] = (2.0f * p0 - 2.0f * p1 + t1 + t0) * ttt + (3.0f * p1 - 3.0f * p0 - 2.0f * t0 - t1) * tt + t0 * t + p0;
		}
	}

	float fittSplineKeys( Spline* pSpline, size_t dimensions, const float* pSource, size_t length, float maxError )
	{
		List< SplineKey > keys;

		float maxErrorResult = splineCreateKeys(
			keys,
			pSource,
			length,
			dimensions,
			maxError
		);
		TIKI_ASSERT( keys[ keys.getCount() - 1u ].time == length - 1u );

		createSpline(
			pSpline,
			dimensions,
			length,
			keys.getData(),
			keys.getCount()
		);

		return maxErrorResult;
	}
}