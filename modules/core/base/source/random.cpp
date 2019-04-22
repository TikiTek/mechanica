#include "tiki/base/random.hpp"

namespace tiki
{
	// see https://en.wikipedia.org/wiki/Permuted_congruential_generator

	static const uint64 s_multiplier = 6364136223846793005u;
	static const uint64 s_increment  = 1442695040888963407u;	// Or an arbitrary odd constant

	RandomNumberGenerator::RandomNumberGenerator()
	{
		m_state = 0u;
	}

	void RandomNumberGenerator::createFromTime()
	{
		//m_state = createFromSeed( time() );
	}

	void RandomNumberGenerator::createFromSeed( uint64 seed )
	{
		m_state = seed + s_increment;
		next();
	}

	sint8 RandomNumberGenerator::nextSInt8()
	{
		return sint8( next() );
	}

	sint16 RandomNumberGenerator::nextSInt16()
	{
		return sint16( next() );
	}

	sint32 RandomNumberGenerator::nextSInt32()
	{
		return sint32( next() );
	}

	sint64 RandomNumberGenerator::nextSInt64()
	{
		return sint64( next() );
	}

	uint8 RandomNumberGenerator::nextUInt8()
	{
		return uint8( next() );
	}

	uint16 RandomNumberGenerator::nextUInt16()
	{
		return uint16( next() );
	}

	uint32 RandomNumberGenerator::nextUInt32()
	{
		return uint32( next() );
	}

	uint64 RandomNumberGenerator::nextUInt64()
	{
		return uint64( next() );
	}

	uint64 RandomNumberGenerator::nextUniformUInt64( uint64 minValue, uint64 maxValue )
	{
		const uint64 range = (maxValue - minValue);
		return minValue + (next() % range);
	}

	float32 RandomNumberGenerator::nextNormalizedFloat32()
	{
		return float32( next() ) / 18446744073709551616.0f;
	}

	float64 RandomNumberGenerator::nextNormalizedFloat64()
	{
		return float64( next() ) / 18446744073709551616.0;
	}

	float32 RandomNumberGenerator::nextUniformFloat32( float32 minValue, float32 maxValue )
	{
		return (nextNormalizedFloat32() + minValue) * (maxValue - minValue);
	}

	float64 RandomNumberGenerator::nextUniformFloat64( float64 minValue, float64 maxValue )
	{
		return (nextNormalizedFloat64() + minValue) * (maxValue - minValue);
	}

	uint64 RandomNumberGenerator::next()
	{
		uint64 x = m_state;
		const uint64 count = (x >> 59u);

		m_state = x * s_multiplier + s_increment;

		x ^= x >> (5u + count);
		x *= 12605985483714917081u;

		return x ^ (x >> 43u);
	}
}
