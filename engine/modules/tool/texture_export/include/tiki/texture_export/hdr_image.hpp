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

		void			create( const uintreg width, const uintreg height, GammaType gamma = GammaType_Linear );
		bool			createFromFile( const char* pFileName );
		void			createFromImage( const HdrImage& imageToCopy );

		void			dispose();

		uintreg			getWidth() const		{ return m_width; }
		uintreg			getHeight() const		{ return m_height; }
		uintreg			getChannelCount() const { return ChannelCount; }
		GammaType		getGammaType() const	{ return m_gammaType; }

		float*			getData() { return m_data.getBegin(); }

		void			resizeImage( uintreg width, uintreg height );
		void			resizeImage( const uint2& size );
		void			cropImage( const uint4& rect );
		void			covertGamma( GammaType gammaType );
		void			flipImage( FlipDirection direction );

		void			convertTo( Array< uint8 >& target, PixelFormat targetFormat ) const;

	private:

		GammaType		m_gammaType;

		uintreg			m_width;
		uintreg			m_height;

		Array< float >	m_data;

		bool			loadPsdFile( const char* pFilename );
		bool			loadPngFile( const char* pFilename );
	};
}
