
#include "tiki/base/float16.hpp"

namespace tiki
{
	union Bits
	{
		float f;
		sint32 si;
		uint32 ui;
	};

	static int const shift = 13;
	static int const shiftSign = 16;

	static const sint32 infN = 0x7F800000; // flt32 infinity
	static const sint32 maxN = 0x477FE000; // max flt16 normal as a flt32
	static const sint32 minN = 0x38800000; // min flt16 normal as a flt32
	static const sint32 signN = 0x80000000; // flt32 sign bit

	static const sint32 infC = infN >> shift;
	static const sint32 nanN = (infC + 1) << shift; // minimum flt16 nan as a flt32
	static const sint32 maxC = maxN >> shift;
	static const sint32 minC = minN >> shift;
	static const sint32 signC = signN >> shiftSign; // flt16 sign bit

	static const sint32 mulN = 0x52000000; // (1 << 23) / minN
	static const sint32 mulC = 0x33800000; // minN / (1 << (23 - shift))

	static const sint32 subC = 0x003FF; // max flt32 subnormal down shifted
	static const sint32 norC = 0x00400; // min flt32 normal down shifted

	static const sint32 maxD = infC - maxC - 1;
	static const sint32 minD = minC - subC - 1;

	float16 f16::convertFloat32to16( float value )
	{
		Bits v, s;
		v.f = value;
		uint32 sign = v.si & signN;
		v.si ^= sign;
		sign >>= shiftSign; // logical shift
		s.si = mulN;
		s.si = (sint32)( s.f * v.f ); // correct subnormals
		v.si ^= (s.si ^ v.si) & -(minN > v.si);
		v.si ^= (infN ^ v.si) & -((infN > v.si) & (v.si > maxN));
		v.si ^= (nanN ^ v.si) & -((nanN > v.si) & (v.si > infN));
		v.ui >>= shift; // logical shift
		v.si ^= ((v.si - maxD) ^ v.si) & -(v.si > maxC);
		v.si ^= ((v.si - minD) ^ v.si) & -(v.si > subC);
		return v.ui | sign;
	}

	float f16::convertFloat16to32( float16 value )
	{
		Bits v;
		v.ui = value;
		sint32 sign = v.si & signC;
		v.si ^= sign;
		sign <<= shiftSign;
		v.si ^= ((v.si + minD) ^ v.si) & -(v.si > subC);
		v.si ^= ((v.si + maxD) ^ v.si) & -(v.si > maxC);
		Bits s;
		s.si = mulC;
		s.f *= v.si;
		sint32 mask = -(norC > v.si);
		v.si <<= shift;
		v.si ^= (s.si ^ v.si) & mask;
		v.si |= sign;
		return v.f;
	}
}