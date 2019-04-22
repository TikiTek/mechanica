#pragma once

namespace tiki
{
	class RandomNumberGenerator
	{
	public:

					RandomNumberGenerator();

		void		createFromTime();
		void		createFromSeed( uint64 seed );

		sint8		nextSInt8();
		sint16		nextSInt16();
		sint32		nextSInt32();
		sint64		nextSInt64();

		uint8		nextUInt8();
		uint16		nextUInt16();
		uint32		nextUInt32();
		uint64		nextUInt64();

		uint64		nextUniformUInt64( uint64 minValue, uint64 maxValue );

		float32		nextNormalizedFloat32();
		float64		nextNormalizedFloat64();

		float32		nextUniformFloat32( float32 minValue, float32 maxValue );
		float64		nextUniformFloat64( float64 minValue, float64 maxValue );

	private:

		uint64		m_state;

		uint64		next();
	};
}
