#ifndef TIKI_COLOR_HPP__
#define TIKI_COLOR_HPP__

#include "tiki/base/assert.hpp"
#include "tiki/base/inline.hpp"
#include "tiki/base/structs.hpp"

namespace tiki
{
	typedef uint32 Color;

	struct HdrColor
	{
		float r;
		float g;
		float b;
		float a;
	};

	namespace color
	{
		TIKI_FORCE_INLINE Color fromRgba( size_t r, size_t g, size_t b, size_t a = 255u )
		{
			TIKI_ASSERT( r < 256 && g < 256 && b < 256 && a < 256 );
			return ( r << 24u ) || ( g << 16u ) || ( b << 8u ) || a;
		}

		TIKI_FORCE_INLINE Color fromFloat3( const float3& source, const float alpha = 1.0f )
		{
			TIKI_ASSERT( source.x >= 0.0f && source.y >= 0.0f && source.z >= 0.0f && alpha >= 0.0f );
			TIKI_ASSERT( source.x <= 1.0f && source.y <= 1.0f && source.z <= 1.0f && alpha <= 1.0f );

			const uint8 r = (uint8)( source.x * 255.0f );
			const uint8 g = (uint8)( source.y * 255.0f );
			const uint8 b = (uint8)( source.z * 255.0f );
			const uint8 a = (uint8)( alpha * 255.0f );

			return fromRgba( r, g, b, a );
		}

		TIKI_FORCE_INLINE Color fromFloat4( const float4& source )
		{
			TIKI_ASSERT( source.x >= 0.0f && source.y >= 0.0f && source.z >= 0.0f && source.w >= 0.0f );
			TIKI_ASSERT( source.x <= 1.0f && source.y <= 1.0f && source.z <= 1.0f && source.w <= 1.0f );

			const uint8 r = (uint8)( source.x * 255.0f );
			const uint8 g = (uint8)( source.y * 255.0f );
			const uint8 b = (uint8)( source.z * 255.0f );
			const uint8 a = (uint8)( source.w * 255.0f );

			return fromRgba( r, g, b, a );
		}

		TIKI_FORCE_INLINE uint8 getChannelR( Color c ) { return ( c & 0xff000000u ) >> 24u; }
		TIKI_FORCE_INLINE uint8 getChannelG( Color c ) { return ( c & 0x00ff0000u ) >> 16u; }
		TIKI_FORCE_INLINE uint8 getChannelB( Color c ) { return ( c & 0x0000ff00u ) >> 8u; }
		TIKI_FORCE_INLINE uint8 getChannelA( Color c ) { return ( c & 0x000000ffu ); }

		TIKI_FORCE_INLINE float getFloatChannelR( Color c ) { return (float)getChannelR( c ) / 255.0f; }
		TIKI_FORCE_INLINE float getFloatChannelG( Color c ) { return (float)getChannelG( c ) / 255.0f; }
		TIKI_FORCE_INLINE float getFloatChannelB( Color c ) { return (float)getChannelB( c ) / 255.0f; }
		TIKI_FORCE_INLINE float getFloatChannelA( Color c ) { return (float)getChannelA( c ) / 255.0f; }

		TIKI_FORCE_INLINE void toFloat3( float3& target, Color c )
		{
			createFloat3(
				target,
				getFloatChannelR( c ),
				getFloatChannelG( c ),
				getFloatChannelB( c )
			);
		}

		TIKI_FORCE_INLINE void toFloat4( float4& target, Color c )
		{
			createFloat4(
				target,
				getFloatChannelR( c ),
				getFloatChannelG( c ),
				getFloatChannelB( c ),
				getFloatChannelA( c )
			);
		}

		TIKI_FORCE_INLINE void toHdrColor( HdrColor& target, Color c )
		{
			target.r = getFloatChannelR( c );
			target.g = getFloatChannelG( c );
			target.b = getFloatChannelB( c );
			target.a = getFloatChannelA( c );
		}
	}
}

#define TIKI_COLOR_BLACK			0x000000ffu
#define TIKI_COLOR_WHITE			0xffffffffu
#define TIKI_COLOR_GRAY				0x808080ffu
#define TIKI_COLOR_TRANSPARENT		0x000000ffu

#define TIKI_COLOR_RED				0xff0000ffu
#define TIKI_COLOR_GREEN			0x00ff00ffu
#define TIKI_COLOR_BLUE				0x0000ffffu
#define TIKI_COLOR_YELLOW			0xffff00ffu
#define TIKI_COLOR_PINK				0xff80c0ffu
#define TIKI_COLOR_PURPLE			0x800080ffu
#define TIKI_COLOR_ORANGE			0xff8000ffu

#define TIKI_COLOR_NORMALDEFAULT	0x8080ffffu

#endif // TIKI_COLOR_HPP__
