#pragma once

#include "tiki/base/types.hpp"
#include "tiki/container/array.hpp"
#include "tiki/graphics/pixel_format.hpp"

namespace tiki
{

	struct uint2;
	struct uint4;

	class HdrImage
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( HdrImage );

	public:

		enum
		{
			ChannelCount	= 4u
		};

		enum GammaType
		{
			GammaType_SRGB,
			GammaType_Linear
		};

		enum FlipDirection
		{
			FlipDirection_Horizontal,
			FlipDirection_Vertical
		};

		void			create( const size_t width, const size_t height, GammaType gamma = GammaType_Linear );
		bool			createFromFile( const char* pFileName );
		void			createFromImage( const HdrImage& imageToCopy );

		void			dispose();

		size_t			getWidth() const		{ return m_width; }
		size_t			getHeight() const		{ return m_height; }
		size_t			getChannelCount() const { return ChannelCount; }

		float*			getData() { return m_data.getBegin(); }

		void			resizeImage( uint width, uint height );
		void			resizeImage( const uint2& size );
		void			cropImage( const uint4& rect );
		void			covertGamma( GammaType gammaType );
		void			flipImage( FlipDirection direction );

		void			convertTo( Array< uint8 >& target, PixelFormat targetFormat ) const;

	private:

		GammaType		m_gammaType;

		size_t			m_width;
		size_t			m_height;

		Array< float >	m_data;

		bool			loadPsdFile( const char* pFilename );
		bool			loadPngFile( const char* pFilename );
	};
}
