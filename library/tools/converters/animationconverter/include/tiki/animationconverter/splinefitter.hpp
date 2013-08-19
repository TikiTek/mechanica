#ifndef TIKI_SPLINEFITTER_HPP
#define TIKI_SPLINEFITTER_HPP

#include "tiki/base/types.hpp"
#include "tiki/base/array.hpp"

namespace tiki
{
	struct SplineKey
	{
		uint32 time;

		float values[ 4u ];
		float controls[ 4u ];
	};

	struct Spline
	{
		size_t				dimensions;
		size_t				length;

		Array< SplineKey > keyData;
	};

	struct SplineErrorResult
	{
		float maxError;
		float errorVolume;
	};

	void	createSpline( Spline* pSpline, size_t dimensions, size_t length, const SplineKey* pKeys, size_t keyCount );
	void	destroySpline( Spline* pSpline );

	void	sampleSpline( float* pTarget, const Spline* pSpline, float time );
	void	sampleSpline( float* pTarget, size_t dimensions, const float* pPoint0, const float* pPoint1, const float* pTangent0, const float* pTangent1, float tangentScale, float time );

	float	fittSplineKeys( Spline* pSpline, size_t dimensions, const float* pSource, size_t length, float maxError );
}

#endif // TIKI_SPLINEFITTER_HPP
