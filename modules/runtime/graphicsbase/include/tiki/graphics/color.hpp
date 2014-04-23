#ifndef TIKI_COLOR_HPP__
#define TIKI_COLOR_HPP__

#include "tiki/base/types.hpp"
#include "tiki/base/assert.hpp"
#include "tiki/base/inline.hpp"
#include "tiki/base/structs.hpp"

#if TIKI_ENABLED( TIKI_BUILD_TOOLS )
#	define TIKI_COLORMODE_RGBA	TIKI_ON
#else
#	if TIKI_ENABLED( TIKI_GRAPHICS_D3D11 )
#		define TIKI_COLORMODE_ABGR	TIKI_ON
#	endif
#endif

#ifndef TIKI_COLORMODE_RGBA
#	define TIKI_COLORMODE_RGBA TIKI_OFF
#endif

#ifndef TIKI_COLORMODE_BGRA
#	define TIKI_COLORMODE_BGRA TIKI_OFF
#endif

#ifndef TIKI_COLORMODE_ARGB
#	define TIKI_COLORMODE_ARGB TIKI_OFF
#endif

#ifndef TIKI_COLORMODE_ABGR
#	define TIKI_COLORMODE_ABGR TIKI_OFF
#endif

#if TIKI_ENABLED( TIKI_COLORMODE_RGBA )
#	define TIKI_COLOR_OFFSET_RED	24u
#	define TIKI_COLOR_OFFSET_GREEN	16u
#	define TIKI_COLOR_OFFSET_BLUE	 8u
#	define TIKI_COLOR_OFFSET_ALPHA	 0u
#elif TIKI_ENABLED( TIKI_COLORMODE_BGRA )
#	define TIKI_COLOR_OFFSET_RED	 8u
#	define TIKI_COLOR_OFFSET_GREEN	16u
#	define TIKI_COLOR_OFFSET_BLUE	24u
#	define TIKI_COLOR_OFFSET_ALPHA	 0u
#elif TIKI_ENABLED( TIKI_COLORMODE_ARGB )
#	define TIKI_COLOR_OFFSET_RED	16u
#	define TIKI_COLOR_OFFSET_GREEN	 8u
#	define TIKI_COLOR_OFFSET_BLUE	 0u
#	define TIKI_COLOR_OFFSET_ALPHA	24u
#elif TIKI_ENABLED( TIKI_COLORMODE_ABGR )
#	define TIKI_COLOR_OFFSET_RED	 0u
#	define TIKI_COLOR_OFFSET_GREEN	 8u
#	define TIKI_COLOR_OFFSET_BLUE	16u
#	define TIKI_COLOR_OFFSET_ALPHA	24u
#endif

#define TIKI_COLOR_MASK_RED			( 0xffu << TIKI_COLOR_OFFSET_RED )
#define TIKI_COLOR_MASK_GREEN		( 0xffu << TIKI_COLOR_OFFSET_GREEN )
#define TIKI_COLOR_MASK_BLUE		( 0xffu << TIKI_COLOR_OFFSET_BLUE )
#define TIKI_COLOR_MASK_ALPHA		( 0xffu << TIKI_COLOR_OFFSET_ALPHA )

#define TIKI_COLOR( r, g, b, a )	( ( ( r ) << TIKI_COLOR_OFFSET_RED ) | ( ( g ) << TIKI_COLOR_OFFSET_GREEN )  | ( ( b ) << TIKI_COLOR_OFFSET_BLUE )  | ( ( a ) << TIKI_COLOR_OFFSET_ALPHA ) )
#define TIKI_COLOR_RGB( r, g, b )	( ( ( r ) << TIKI_COLOR_OFFSET_RED ) | ( ( g ) << TIKI_COLOR_OFFSET_GREEN )  | ( ( b ) << TIKI_COLOR_OFFSET_BLUE )  | ( 255u << TIKI_COLOR_OFFSET_ALPHA ) )

#define TIKI_COLOR_BLACK			TIKI_COLOR( 0x00u, 0x00u, 0x00u, 0xffu )
#define TIKI_COLOR_WHITE			TIKI_COLOR( 0xffu, 0xffu, 0xffu, 0xffu )
#define TIKI_COLOR_GRAY				TIKI_COLOR( 0x80u, 0x80u, 0x80u, 0xffu )
#define TIKI_COLOR_TRANSPARENT		TIKI_COLOR( 0x00u, 0x00u, 0x00u, 0x00u )

#define TIKI_COLOR_RED				TIKI_COLOR( 0xffu, 0x00u, 0x00u, 0xffu )
#define TIKI_COLOR_GREEN			TIKI_COLOR( 0x00u, 0xffu, 0x00u, 0xffu )
#define TIKI_COLOR_BLUE				TIKI_COLOR( 0x00u, 0x00u, 0xffu, 0xffu )
#define TIKI_COLOR_YELLOW			TIKI_COLOR( 0xffu, 0xffu, 0x00u, 0xffu )
#define TIKI_COLOR_PINK				TIKI_COLOR( 0xffu, 0x80u, 0xc0u, 0xffu )
#define TIKI_COLOR_PURPLE			TIKI_COLOR( 0x80u, 0x00u, 0x80u, 0xffu )
#define TIKI_COLOR_ORANGE			TIKI_COLOR( 0xffu, 0x80u, 0x00u, 0xffu )

#define TIKI_COLOR_NORMALDEFAULT	TIKI_COLOR( 0x80u, 0x80u, 0xffu, 0xffu )

namespace tiki
{
	typedef uint32 Color;

	struct HdrColor
	{
#if TIKI_ENABLED( TIKI_COLORMODE_RGBA )
		float r, g, b, a;
#elif TIKI_ENABLED( TIKI_COLORMODE_BGRA )
		float b, g, r, a;
#elif TIKI_ENABLED( TIKI_COLORMODE_ARGB )
		float a, r, g, b;
#elif TIKI_ENABLED( TIKI_COLORMODE_ABGR )
		float a, b, g, r;
#endif
	};

	namespace color
	{
		TIKI_FORCE_INLINE Color fromRGBA( size_t r, size_t g, size_t b, size_t a = 255u )
		{
			TIKI_ASSERT( r < 256 && g < 256 && b < 256 && a < 256 );
			return ( r << TIKI_COLOR_OFFSET_RED ) || ( g << TIKI_COLOR_OFFSET_GREEN ) || ( b << TIKI_COLOR_OFFSET_BLUE ) || ( a << TIKI_COLOR_OFFSET_ALPHA );
		}

		TIKI_FORCE_INLINE Color fromFloat3( const float3& source, const float alpha = 1.0f )
		{
			TIKI_ASSERT( source.x >= 0.0f && source.y >= 0.0f && source.z >= 0.0f && alpha >= 0.0f );
			TIKI_ASSERT( source.x <= 1.0f && source.y <= 1.0f && source.z <= 1.0f && alpha <= 1.0f );

			const uint8 r = (uint8)( source.x * 255.0f );
			const uint8 g = (uint8)( source.y * 255.0f );
			const uint8 b = (uint8)( source.z * 255.0f );
			const uint8 a = (uint8)( alpha * 255.0f );

			return fromRGBA( r, g, b, a );
		}

		TIKI_FORCE_INLINE Color fromFloat4( const float4& source )
		{
			TIKI_ASSERT( source.x >= 0.0f && source.y >= 0.0f && source.z >= 0.0f && source.w >= 0.0f );
			TIKI_ASSERT( source.x <= 1.0f && source.y <= 1.0f && source.z <= 1.0f && source.w <= 1.0f );

			const uint8 r = (uint8)( source.x * 255.0f );
			const uint8 g = (uint8)( source.y * 255.0f );
			const uint8 b = (uint8)( source.z * 255.0f );
			const uint8 a = (uint8)( source.w * 255.0f );

			return fromRGBA( r, g, b, a );
		}

		TIKI_FORCE_INLINE uint8 getChannelR( Color c ) { return ( c & TIKI_COLOR_MASK_RED )		>> TIKI_COLOR_OFFSET_RED; }
		TIKI_FORCE_INLINE uint8 getChannelG( Color c ) { return ( c & TIKI_COLOR_MASK_GREEN )	>> TIKI_COLOR_OFFSET_GREEN; }
		TIKI_FORCE_INLINE uint8 getChannelB( Color c ) { return ( c & TIKI_COLOR_MASK_BLUE )	>> TIKI_COLOR_OFFSET_BLUE; }
		TIKI_FORCE_INLINE uint8 getChannelA( Color c ) { return ( c & TIKI_COLOR_MASK_ALPHA )	>> TIKI_COLOR_OFFSET_ALPHA; }

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

#endif // TIKI_COLOR_HPP__
