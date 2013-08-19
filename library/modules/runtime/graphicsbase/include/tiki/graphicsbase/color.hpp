#ifndef TIKI_COLOR_HPP__
#define TIKI_COLOR_HPP__

#include "tiki/base/inline.hpp"

namespace tiki
{
	struct Vector3;
	struct Vector4;

	struct float3;
	struct float4;

	struct Color
	{
		union
		{
			struct  
			{
				float r;
				float g;
				float b;
				float a;
			};
			float arr[ 4u ];
		};

		Color();
		Color( const Color& color );

		Color& operator=( const Color& color );

		Color( float r,float g, float b, float a );

		TIKI_FORCE_INLINE void	fromVector3( const Vector3& source, const float alpha = 1.0f );
		TIKI_FORCE_INLINE void	fromVector4( const Vector4& source );

		TIKI_FORCE_INLINE void	toVector3( Vector3& target ) const;
		TIKI_FORCE_INLINE void	toVector4( Vector4& target ) const;

		TIKI_FORCE_INLINE void	toFloat3( float3& target ) const;
		TIKI_FORCE_INLINE void	toFloat4( float4& target ) const;

		static Color transparent;
		static Color normalDefault;
		static Color white;
		static Color black;
		static Color red;
		static Color pink;
		static Color green;
		static Color blue;
		static Color yellow;
		static Color tikiBlue;
		static Color cornFlowerBlue;
		static Color gray;
		static Color purple;
		static Color orange;
	};
}

#include "color.inl"

#endif // TIKI_COLOR_HPP__
