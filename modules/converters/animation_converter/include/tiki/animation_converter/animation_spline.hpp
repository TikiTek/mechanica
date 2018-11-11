#pragma once

#include "tiki/base/types.hpp"
#include "tiki/container/list.hpp"

namespace tiki
{
	enum
	{
		SplineLimits_MaxDimensions = 4u
	};

	struct AnimationSplineKey
	{
		uint	time;

		float	aValues[ SplineLimits_MaxDimensions ];
		float	aControls[ SplineLimits_MaxDimensions ];

		static void	sampleHermite( float* pTarget, uint targetCapacity, const AnimationSplineKey& leftKey, const AnimationSplineKey& rightKey, float time );
	};

	class AnimationSpline
	{
		TIKI_NONCOPYABLE_CLASS( AnimationSpline );

	public:

									AnimationSpline();
									~AnimationSpline();

		void						createFromKeys( const AnimationSplineKey* pSourceKeys, uint keyCount, uint dimensions, uint lengthInFrames );
		void						createFromSamples( const float* pSource, uint dimensions, uint lengthInFrames, float threshold ); // source capacity must be dimensions * lengthInFrames
		void						dispose();

		uint						getLengthInFrames() const	{ return m_lengthImFrames; }
		uint						getDimentions() const		{ return m_dimensions; }

		const AnimationSplineKey*	getKeys() const						{ return m_keys.getBegin(); }
		const AnimationSplineKey&	getKeyByIndex( uint index ) const	{ return m_keys[ index ]; }
		uint						getKeyCount() const { return m_keys.getCount(); }

		void						sample( float* pTarget, uint targetCapacity, float sampleTime ) const;

	private:

		uint						m_lengthImFrames;
		uint						m_dimensions;

		List< AnimationSplineKey >	m_keys;

		void						insertKey( const AnimationSplineKey& key );
		AnimationSplineKey			createKey( const float* pSource, uint time, uint dimentions );
		void						calculateTangents( const float* pSource, uint sourceLength, uint dimentions );
	};
}
