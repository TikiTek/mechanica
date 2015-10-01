#pragma once
#ifndef __TIKI_SPLINE_HPP_INCLUDED__
#define __TIKI_SPLINE_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/container/list.hpp"

namespace tiki
{
	enum
	{
		SplineLimits_MaxDimensions = 4u
	};

	struct SplineKey
	{
		uint	time;

		float	aValues[ SplineLimits_MaxDimensions ];
		float	aControls[ SplineLimits_MaxDimensions ];

		static void	sampleHermite( float* pTarget, uint targetCapacity, const SplineKey& leftKey, const SplineKey& rightKey, float time );
	};

	class Spline
	{
		TIKI_NONCOPYABLE_CLASS( Spline );

	public:

		Spline();
		~Spline();

		void				createFromKeys( const SplineKey* pSourceKeys, uint keyCount, uint dimensions, uint lengthInFrames );
		void				createFromSamples( const float* pSource, uint dimensions, uint lengthInFrames, float threshold ); // source capacity must be dimensions * lengthInFrames
		void				dispose();

		uint				getLengthInFrames() const	{ return m_lengthImFrames; }
		uint				getDimentions() const		{ return m_dimensions; }

		const SplineKey*	getKeys() const						{ return m_keys.getBegin(); }
		const SplineKey&	getKeyByIndex( uint index ) const	{ return m_keys[ index ]; }
		uint				getKeyCount() const					{ return m_keys.getCount(); }

		void				sample( float* pTarget, uint targetCapacity, float sampleTime ) const;

	private:

		uint				m_lengthImFrames;
		uint				m_dimensions;

		List< SplineKey >	m_keys;

		void				insertKey( const SplineKey& key );
		SplineKey			createKey( const float* pSource, uint time, uint dimentions );
		void				calculateTangents( const float* pSource, uint sourceLength, uint dimentions );

	};
}

#endif // __TIKI_SPLINE_HPP_INCLUDED__
