
#include "tiki/textureexport/hdrimage.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/float32.hpp"
#include "tiki/base/sizedarray.hpp"
#include "tiki/base/structs.hpp"

#include "libpsd.h"
#include "opencv2/opencv.hpp"

namespace tiki
{
	void HdrImage::create( const size_t width, const size_t height, GammaType gamma /*= GammaType_Linear */ )
	{
		m_gammaType	= gamma;

		m_width		= width;
		m_height	= height;

		m_data.create( width * height * ChannelCount );
		memory::zero( m_data.getData(), m_data.getCount() * sizeof( float ) );
	}

	void HdrImage::createFromImage( const HdrImage& imageToCopy )
	{
		m_gammaType	= imageToCopy.m_gammaType;

		m_width		= imageToCopy.m_width;
		m_height	= imageToCopy.m_height;

		m_data.create( imageToCopy.m_data.getData(), imageToCopy.m_data.getCount() );
	}

	bool HdrImage::createFromFile( const string& fileName )
	{
		psd_context* pContext	= nullptr;
		psd_status status		= psd_image_load( &pContext, (psd_char*)fileName.cStr() );

		if ( status != psd_status_done )
		{
			TIKI_TRACE_ERROR( "input file can't parse: %s\n", fileName.cStr() );
			return false;
		}

		status = psd_image_blend( pContext, 0, 0, pContext->width, pContext->height );

		if ( status != psd_status_done )
		{
			TIKI_TRACE_ERROR( "psd blending failed\n" );
			return false;
		}

		m_gammaType = ( pContext->color_mode == psd_color_mode_rgb ? GammaType_SRGB : GammaType_Linear );
		m_width		= pContext->width;
		m_height	= pContext->height;

		m_data.create( m_width * m_height * ChannelCount );

		const uint8* pPixelData = (const uint8*)pContext->blending_image_data;
		for (size_t i = 0u; i < m_data.getCount(); ++i)
		{
			m_data[ i ] = (float)pPixelData[ i ] / 255.0f;
		}

		psd_image_free( pContext );

		return true;
	}

	void HdrImage::dispose()
	{
		m_data.dispose();
	}

	void HdrImage::resizeImage( const uint2& scale )
	{
		resizeImage( scale.x, scale.y );
	}

	void HdrImage::resizeImage( uint width, uint height )
	{
		Array< float > tempImage;
		tempImage.create( width * height * ChannelCount );

		IplImage* pSrcImage = cvCreateImageHeader( cvSize( m_width, m_height ), IPL_DEPTH_32F, 4 );
		cvSetData( pSrcImage, m_data.getData(), m_width * sizeof( float ) * 4 );

		IplImage* pDestImage = cvCreateImageHeader( cvSize( width, height ), IPL_DEPTH_32F, 4 );
		cvSetData( pDestImage, tempImage.getData(), width * sizeof( float ) * 4 );

		cvResize( pSrcImage, pDestImage, CV_INTER_AREA );

		//cvNamedWindow( "Image", 1 );
		//cvShowImage( "Image", pDestImage );
		//cvWaitKey( 0 );
		//cvDestroyAllWindows();

		cvReleaseImageHeader( &pSrcImage );
		cvReleaseImageHeader( &pDestImage );

		m_data.dispose();
		m_data.create( tempImage.getData(), tempImage.getCount() );

		m_width		= width;
		m_height	= height;

		tempImage.dispose();
	}

	void HdrImage::cropImage( const uint4& rect )
	{
		TIKI_ASSERT( rect.x + rect.z <= m_width );
		TIKI_ASSERT( rect.y + rect.w <= m_height );

		SizedArray< float > tempImage;
		tempImage.create( rect.z * rect.w * ChannelCount );

		for (size_t i = 0u; i < rect.w; ++i)
		{
			const float* row = m_data.getData() + ( rect.y * m_width * ChannelCount ) + ( i * m_width * ChannelCount ) + ( rect.x * ChannelCount );

			tempImage.pushRange( row, rect.z * ChannelCount );
		}

		m_data.dispose();
		m_data.create( tempImage.getData(), tempImage.getCount() );

		m_width		= rect.z;
		m_height	= rect.w;

		tempImage.dispose();
	}

	void HdrImage::covertGamma( const GammaType gammaType )
	{
		TIKI_ASSERT( m_gammaType != gammaType );

		float powVal = 0.0f;

		if ( m_gammaType == GammaType_SRGB && gammaType == GammaType_Linear )
		{
			powVal = 0.45f;
		}
		else if ( m_gammaType == GammaType_Linear && gammaType == GammaType_SRGB )
		{
			powVal = 2.2f;
		}
		else
		{
			TIKI_BREAK( "case not implemented" );
		}

		for (size_t i = 0u; i < m_data.getCount(); ++i)
		{
			m_data[ i ] = f32::pow( m_data[ i ], powVal );
		}
	}

	void HdrImage::convertTo( Array< uint8 >& target, const PixelFormat targetFormat ) const
	{
		switch ( targetFormat )
		{
		case PixelFormat_R8:
			{
				target.create( m_width * m_height );
				uint8* pTargetData = (uint8*)target.getData();

				for (size_t i = 0u; i < m_data.getCount(); i += ChannelCount)
				{
					pTargetData[ i / ChannelCount ] = (uint8)(m_data[ i ] * 255.0f);
				}
			}
			break;
		case PixelFormat_R8G8B8A8:
		case PixelFormat_R8G8B8A8_Gamma:
			{
				target.create( m_width * m_height * ChannelCount );
				uint8* pTargetData = (uint8*)target.getData();

				for (size_t i = 0u; i < m_data.getCount(); i += ChannelCount)
				{
					pTargetData[ i + 0u ] = (uint8)(m_data[ i + 2u ] * 255.0f);
					pTargetData[ i + 1u ] = (uint8)(m_data[ i + 1u ] * 255.0f);
					pTargetData[ i + 2u ] = (uint8)(m_data[ i + 0u ] * 255.0f);
					pTargetData[ i + 3u ] = (uint8)(m_data[ i + 3u ] * 255.0f);
				}
			}
			break;
		case PixelFormat_R32G32B32_Float:
			{
				target.create( m_width * m_height * 3u * sizeof( float ) );
				float* pTargetData = (float*)target.getData();

				for (size_t i = 0u; i < m_data.getCount(); i += ChannelCount)
				{
					const size_t targetIndex = ( i / 4u ) * 3u;

					pTargetData[ targetIndex + 0u ] = m_data[ i + 2u ];
					pTargetData[ targetIndex + 1u ] = m_data[ i + 1u ];
					pTargetData[ targetIndex + 2u ] = m_data[ i + 0u ];
				}
			}
			break;
		case PixelFormat_R32G32B32A32_Float:
			{
				target.create( m_width * m_height * 4u * sizeof( float ) );
				float* pTargetData = (float*)target.getData();

				for (size_t i = 0u; i < m_data.getCount(); i += ChannelCount)
				{
					pTargetData[ i + 0u ] = m_data[ i + 2u ];
					pTargetData[ i + 1u ] = m_data[ i + 1u ];
					pTargetData[ i + 2u ] = m_data[ i + 0u ];
					pTargetData[ i + 3u ] = m_data[ i + 3u ];
				}
			}
			break;
		default:
			TIKI_BREAK( "[textureconverter] Unknown PixelFormat.\n" );
			break;
		}
	}
}