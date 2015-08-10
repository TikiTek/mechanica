
#include "tiki/textureexport/hdrimage.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/float32.hpp"
#include "tiki/base/sizedarray.hpp"
#include "tiki/graphics/color.hpp"

#include "libpsd.h"

#include "base.hpp"

namespace tiki
{
	void HdrImage::create( const size_t width, const size_t height, GammaType gamma /*= GammaType_Linear */ )
	{
		m_gammaType	= gamma;

		m_width		= width;
		m_height	= height;

		m_data.create( width * height * ChannelCount );
		memory::zero( m_data.getBegin(), m_data.getCount() * sizeof( float ) );
	}

	void HdrImage::createFromImage( const HdrImage& imageToCopy )
	{
		m_gammaType	= imageToCopy.m_gammaType;

		m_width		= imageToCopy.m_width;
		m_height	= imageToCopy.m_height;

		m_data.create( imageToCopy.m_data.getBegin(), imageToCopy.m_data.getCount() );
	}

	bool HdrImage::createFromFile( cstring pFileName )
	{
		psd_context* pContext	= nullptr;
		psd_status status		= psd_image_load( &pContext, (psd_char*)pFileName );

		if ( status != psd_status_done )
		{
			TIKI_TRACE_ERROR( "input file can't parse: %s\n", pFileName );
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

	void HdrImage::resizeImage( uint targetWidth, uint targetHeight )
	{
		if ( targetWidth == m_width && targetHeight == m_height )
		{
			return;
		}

		Array< HdrColor > tempImage;
		tempImage.create( targetWidth * targetHeight );

		// source: http://paint-mono.googlecode.com/svn/trunk/src/PdnLib/Surface.cs
		HdrColor* pSourceData = static_cast< HdrColor* >( static_cast< void* >( m_data.getBegin() ) );

		for (uint dstY = 0u; dstY < targetHeight; ++dstY)
		{
			double srcTop = (double)( dstY * m_height ) / (double)targetHeight;
			double srcTopFloor = floor( srcTop );
			double srcTopWeight = 1 - (srcTop - srcTopFloor);
			int srcTopInt = (int)srcTopFloor;

			double srcBottom = (double)((dstY + 1) * m_height) / (double)targetHeight;
			double srcBottomFloor = floor( srcBottom - 0.00001 );
			double srcBottomWeight = srcBottom - srcBottomFloor;
			int srcBottomInt = (int)srcBottomFloor;

			const uint destRowIndex = ( dstY * targetWidth );
			HdrColor* pDest = &tempImage[ destRowIndex ];

			for (uint dstX = 0u; dstX < targetWidth; ++dstX)
			{
				double srcLeft = (double)(dstX * m_width) / (double)targetWidth;
				double srcLeftFloor = floor( srcLeft );
				double srcLeftWeight = 1.0 - ( srcLeft - srcLeftFloor );
				int srcLeftInt = (int)srcLeftFloor;

				double srcRight = (double)((dstX + 1) * m_width) / (double)targetWidth;
				double srcRightFloor = floor( srcRight - 0.00001 );
				double srcRightWeight = srcRight - srcRightFloor;
				int srcRightInt = (int)srcRightFloor;

				double blueSum = 0;
				double greenSum = 0;
				double redSum = 0;
				double alphaSum = 0;

				// left fractional edge
				const uint sourceLeftIndex = ( ( srcTopInt + 1u ) * m_width ) + srcLeftInt;
				HdrColor* pSourceLeft = &pSourceData[ sourceLeftIndex ];

				for (int srcY = srcTopInt + 1; srcY < srcBottomInt; ++srcY)
				{
					double a	 = pSourceLeft->a;
					blueSum		+= pSourceLeft->b * srcLeftWeight * a;
					greenSum	+= pSourceLeft->g * srcLeftWeight * a;
					redSum		+= pSourceLeft->r * srcLeftWeight * a;
					alphaSum	+= pSourceLeft->a * srcLeftWeight;
					pSourceLeft  += m_width;
				}

				// right fractional edge
				const uint sourceRightIndex = ( ( srcTopInt + 1u ) * m_width ) + srcRightInt;
				HdrColor* pSourceRight = &pSourceData[ sourceRightIndex ];
				for (int srcY = srcTopInt + 1; srcY < srcBottomInt; ++srcY)
				{
					double a		 = pSourceRight->a;
					blueSum			+= pSourceRight->b * srcRightWeight * a;
					greenSum		+= pSourceRight->g * srcRightWeight * a;
					redSum			+= pSourceRight->r * srcRightWeight * a;
					alphaSum		+= pSourceRight->a * srcRightWeight;
					pSourceRight	+= m_width;
				}

				// top fractional edge
				const uint sourceTopIndex = ( srcTopInt * m_width ) + ( srcLeftInt + 1u );
				HdrColor* pSourceTop = &pSourceData[ sourceTopIndex ];
				for (int srcX = srcLeftInt + 1; srcX < srcRightInt; ++srcX)
				{
					double a	 = pSourceTop->a;
					blueSum		+= pSourceTop->b * srcTopWeight * a;
					greenSum	+= pSourceTop->g * srcTopWeight * a;
					redSum		+= pSourceTop->r * srcTopWeight * a;
					alphaSum	+= pSourceTop->a * srcTopWeight;
					++pSourceTop;
				}

				// bottom fractional edge
				const uint sourceBottomIndex = ( srcBottomInt * m_width ) + ( srcLeftInt + 1u );
				HdrColor* pSourceBottom = &pSourceData[ sourceBottomIndex ];
				for (int srcX = srcLeftInt + 1; srcX < srcRightInt; ++srcX)
				{
					double a	 = pSourceBottom->a;
					blueSum		+= pSourceBottom->b * srcBottomWeight * a;
					greenSum	+= pSourceBottom->g * srcBottomWeight * a;
					redSum		+= pSourceBottom->r * srcBottomWeight * a;
					alphaSum	+= pSourceBottom->a * srcBottomWeight;
					++pSourceBottom;
				}

				// center area
				for (int srcY = srcTopInt + 1; srcY < srcBottomInt; ++srcY)
				{
					const uint sourceIndex = ( srcY * m_width ) + ( srcLeftInt + 1u );
					HdrColor* pSource = &pSourceData[ sourceIndex ];

					for (int srcX = srcLeftInt + 1; srcX < srcRightInt; ++srcX)
					{
						double a	 = pSource->a;
						blueSum		+= (double)pSource->b * a;
						greenSum	+= (double)pSource->g * a;
						redSum		+= (double)pSource->r * a;
						alphaSum	+= (double)pSource->a;
						++pSource;
					}
				}

				// four corner pixels
				HdrColor srcTL = pSourceData[ (srcTopInt * m_width ) + srcLeftInt ];
				double srcTLA	 = srcTL.a;
				blueSum			+= srcTL.b * (srcTopWeight * srcLeftWeight) * srcTLA;
				greenSum		+= srcTL.g * (srcTopWeight * srcLeftWeight) * srcTLA;
				redSum			+= srcTL.r * (srcTopWeight * srcLeftWeight) * srcTLA;
				alphaSum		+= srcTL.a * (srcTopWeight * srcLeftWeight);

				HdrColor srcTR = pSourceData[ (srcTopInt * m_width ) + srcRightInt ];
				double srcTRA	 = srcTR.a;
				blueSum			+= srcTR.b * (srcTopWeight * srcRightWeight) * srcTRA;
				greenSum		+= srcTR.g * (srcTopWeight * srcRightWeight) * srcTRA;
				redSum			+= srcTR.r * (srcTopWeight * srcRightWeight) * srcTRA;
				alphaSum		+= srcTR.a * (srcTopWeight * srcRightWeight);

				HdrColor srcBL = pSourceData[ (srcBottomInt * m_width ) + srcLeftInt ];
				double srcBLA	 = srcBL.a;
				blueSum			+= srcBL.b * (srcBottomWeight * srcLeftWeight) * srcBLA;
				greenSum		+= srcBL.g * (srcBottomWeight * srcLeftWeight) * srcBLA;
				redSum			+= srcBL.r * (srcBottomWeight * srcLeftWeight) * srcBLA;
				alphaSum		+= srcBL.a * (srcBottomWeight * srcLeftWeight);

				HdrColor srcBR = pSourceData[ (srcBottomInt * m_width ) + srcRightInt ];
				double srcBRA	 = srcBR.a;
				blueSum			+= srcBR.b * (srcBottomWeight * srcRightWeight) * srcBRA;
				greenSum		+= srcBR.g * (srcBottomWeight * srcRightWeight) * srcBRA;
				redSum			+= srcBR.r * (srcBottomWeight * srcRightWeight) * srcBRA;
				alphaSum		+= srcBR.a * (srcBottomWeight * srcRightWeight);

				double area = (srcRight - srcLeft) * (srcBottom - srcTop);

				double alpha = alphaSum / area;
				double blue;
				double green;
				double red;

				if (alpha == 0)
				{
					blue = 0;
					green = 0;
					red = 0;
				}
				else
				{
					blue = blueSum / alphaSum;
					green = greenSum / alphaSum;
					red = redSum / alphaSum;
				}

				// add 0.5 so that rounding goes in the direction we want it to
				blue	+= ( 0.5 / 255.0 );
				green	+= ( 0.5 / 255.0 );
				red		+= ( 0.5 / 255.0 );
				alpha	+= ( 0.5 / 255.0 );

				pDest->r = (float)red; 
				pDest->g = (float)green; 
				pDest->b = (float)blue; 
				pDest->a = (float)alpha; 
				++pDest;
			}
		}

		m_data.dispose();
		m_data.create( (float*)tempImage.getBegin(), tempImage.getCount() * ChannelCount );

		m_width		= targetWidth;
		m_height	= targetHeight;

		tempImage.dispose();
	}

	void HdrImage::cropImage( const uint4& rect )
	{
		TIKI_ASSERT( rect.x + rect.z <= m_width );
		TIKI_ASSERT( rect.y + rect.w <= m_height );

		if ( rect.x == 0u && rect.y == 0u && rect.z == m_width && rect.w == m_height )
		{
			return;
		}

		SizedArray< float > tempImage;
		tempImage.create( rect.z * rect.w * ChannelCount );

		for (size_t i = 0u; i < rect.w; ++i)
		{
			const float* row = m_data.getBegin() + ( rect.y * m_width * ChannelCount ) + ( i * m_width * ChannelCount ) + ( rect.x * ChannelCount );

			tempImage.pushRange( row, rect.z * ChannelCount );
		}

		m_data.dispose();
		m_data.create( tempImage.getBegin(), tempImage.getCount() );

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

	void HdrImage::flipImage( FlipDirection direction )
	{
		SizedArray< float > tempImage;
		tempImage.create( m_data.getCount() );

		const uint lineSize = m_width * ChannelCount;
		switch ( direction )
		{
		case FlipDirection_Horizontal:
			{
				for (uint y = 0u; y < m_height; ++y)
				{
					for (uint x = m_width - 1u; x < m_width; --x)
					{
						const uint lineOffset = x * ChannelCount;
						tempImage.pushRange( &m_data[ y * lineSize + lineOffset ], ChannelCount );
					}
				}
			}
			break;

		case FlipDirection_Vertical:
			{
				for (uint y = m_height - 1u; y < m_height; --y)
				{
					tempImage.pushRange( &m_data[ y * lineSize ], lineSize );
				}
			}
			break;

		default:
			TIKI_BREAK( "case not supported" );
			break;
		}

		m_data.dispose();
		m_data.create( tempImage.getBegin(), tempImage.getCount() );
		tempImage.dispose();
	}

	void HdrImage::convertTo( Array< uint8 >& target, const PixelFormat targetFormat ) const
	{
		switch ( targetFormat )
		{
		case PixelFormat_R8:
			{
				target.create( m_width * m_height );
				uint8* pTargetData = (uint8*)target.getBegin();

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
				uint8* pTargetData = (uint8*)target.getBegin();

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
				float* pTargetData = (float*)target.getBegin();

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
				float* pTargetData = (float*)target.getBegin();

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