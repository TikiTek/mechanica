#include "tiki/texture_export/hdr_image.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/float32.hpp"
#include "tiki/container/sized_array.hpp"
#include "tiki/graphics/color.hpp"
#include "tiki/io/path.hpp"

#include "base.hpp"

#include <libpsd.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace tiki
{
	void HdrImage::create( const uintreg width, const uintreg height, GammaType gamma /*= GammaType_Linear */ )
	{
		m_gammaType	= gamma;

		m_width		= width;
		m_height	= height;

		m_data.create( width * height * ChannelCount, TIKI_DEFAULT_ALIGNMENT, false );
		memory::zero( m_data.getBegin(), m_data.getCount() * sizeof( float ) );
	}

	void HdrImage::createFromImage( const HdrImage& imageToCopy )
	{
		m_gammaType	= imageToCopy.m_gammaType;

		m_width		= imageToCopy.m_width;
		m_height	= imageToCopy.m_height;

		m_data.create( imageToCopy.m_data.getBegin(), imageToCopy.m_data.getCount(), TIKI_DEFAULT_ALIGNMENT, false );
	}

	bool HdrImage::createFromFile( const char* pFileName )
	{
		const DynamicString ext = path::getExtension( pFileName ).toLower();

		if( ext == ".psd" )
		{
			return loadPsdFile( pFileName );
		}
		else if( ext == ".png" )
		{
			return loadPngFile( pFileName );
		}

		return false;
	}

	void HdrImage::dispose()
	{
		m_data.dispose();
	}

	void HdrImage::resizeImage( const uint2& scale )
	{
		resizeImage( scale.x, scale.y );
	}

	void HdrImage::resizeImage( uintreg targetWidth, uintreg targetHeight )
	{
		if ( targetWidth == m_width && targetHeight == m_height )
		{
			return;
		}

		Array< float > tempImage;
		tempImage.create( targetWidth * targetHeight * ChannelCount, TIKI_DEFAULT_ALIGNMENT, false );

		// source: http://paint-mono.googlecode.com/svn/trunk/src/PdnLib/Surface.cs
		HdrColor* pSourceData = static_cast< HdrColor* >( static_cast< void* >( m_data.getBegin() ) );

		for (uintreg dstY = 0u; dstY < targetHeight; ++dstY)
		{
			double srcTop = (double)( dstY * m_height ) / (double)targetHeight;
			double srcTopFloor = floor( srcTop );
			double srcTopWeight = 1 - (srcTop - srcTopFloor);
			int srcTopInt = (int)srcTopFloor;

			double srcBottom = (double)((dstY + 1) * m_height) / (double)targetHeight;
			double srcBottomFloor = floor( srcBottom - 0.00001 );
			double srcBottomWeight = srcBottom - srcBottomFloor;
			int srcBottomInt = (int)srcBottomFloor;

			const uintreg destRowIndex = ( dstY * targetWidth );
			HdrColor* pDest = (HdrColor*)&tempImage[ destRowIndex * ChannelCount ];

			for (uintreg dstX = 0u; dstX < targetWidth; ++dstX)
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
				const uintreg sourceLeftIndex = ( ( srcTopInt + 1u ) * m_width ) + srcLeftInt;
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
				const uintreg sourceRightIndex = ( ( srcTopInt + 1u ) * m_width ) + srcRightInt;
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
				const uintreg sourceTopIndex = ( srcTopInt * m_width ) + ( srcLeftInt + 1u );
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
				const uintreg sourceBottomIndex = ( srcBottomInt * m_width ) + ( srcLeftInt + 1u );
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
					const uintreg sourceIndex = ( srcY * m_width ) + ( srcLeftInt + 1u );
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

		m_data.swap( tempImage );
		tempImage.dispose();

		m_width		= targetWidth;
		m_height	= targetHeight;
	}

	void HdrImage::cropImage( const uint4& rect )
	{
		TIKI_ASSERT( rect.x + rect.z <= m_width );
		TIKI_ASSERT( rect.y + rect.w <= m_height );

		if ( rect.x == 0u && rect.y == 0u && rect.z == m_width && rect.w == m_height )
		{
			return;
		}

		Array< float > tempImage;
		tempImage.create( rect.z * rect.w * ChannelCount, TIKI_DEFAULT_ALIGNMENT, false );

		for (uintreg i = 0u; i < rect.w; ++i)
		{
			const float* row = m_data.getBegin() + ( rect.y * m_width * ChannelCount ) + ( i * m_width * ChannelCount ) + ( rect.x * ChannelCount );
			memory::copy( &tempImage[ i * rect.z * ChannelCount ], row, rect.z * sizeof( float ) * ChannelCount );
		}

		m_data.swap( tempImage );
		tempImage.dispose();

		m_width		= rect.z;
		m_height	= rect.w;
	}

	void HdrImage::covertGamma( const GammaType gammaType )
	{
		if( m_gammaType == gammaType )
		{
			return;
		}

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

		for (uintreg i = 0u; i < m_data.getCount(); ++i)
		{
			m_data[ i ] = f32::pow( m_data[ i ], powVal );
		}
	}

	void HdrImage::flipImage( FlipDirection direction )
	{
		Array< float > tempImage;
		tempImage.create( m_data.getCount(), TIKI_DEFAULT_ALIGNMENT, false );

		const uintreg lineSize = m_width * ChannelCount;
		switch ( direction )
		{
		case FlipDirection_Horizontal:
			{
				float* pTargetData = tempImage.getBegin();
				for (uintreg y = 0u; y < m_height; ++y)
				{
					for (uintreg x = m_width - 1u; x < m_width; --x)
					{
						const uintreg lineOffset = x * ChannelCount;
						memory::copy( pTargetData, &m_data[ y * lineSize + lineOffset ], sizeof( float ) * ChannelCount );
						pTargetData += ChannelCount;
					}
				}
			}
			break;

		case FlipDirection_Vertical:
			{
				float* pTargetData = tempImage.getBegin();
				for (uintreg y = m_height - 1u; y < m_height; --y)
				{
					memory::copy( pTargetData, &m_data[ y * lineSize ], lineSize * sizeof( float ) * ChannelCount );
					pTargetData += lineSize * ChannelCount;
				}
			}
			break;

		default:
			TIKI_BREAK( "case not supported" );
			break;
		}

		m_data.swap( tempImage );
		tempImage.dispose();
	}

	void HdrImage::convertTo( Array< uint8 >& target, const PixelFormat targetFormat ) const
	{
		switch ( targetFormat )
		{
		case PixelFormat_R8:
			{
				target.create( m_width * m_height, TIKI_DEFAULT_ALIGNMENT, false );
				uint8* pTargetData = (uint8*)target.getBegin();

				for (uintreg i = 0u; i < m_data.getCount(); i += ChannelCount)
				{
					pTargetData[ i / ChannelCount ] = (uint8)(m_data[ i ] * 255.0f);
				}
			}
			break;
		case PixelFormat_R8G8B8A8:
		case PixelFormat_R8G8B8A8_Gamma:
			{
				target.create( m_width * m_height * ChannelCount, TIKI_DEFAULT_ALIGNMENT, false );
				uint8* pTargetData = (uint8*)target.getBegin();

				for (uintreg i = 0u; i < m_data.getCount(); i += ChannelCount)
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
				target.create( m_width * m_height * 3u * sizeof( float ), TIKI_DEFAULT_ALIGNMENT, false );
				float* pTargetData = (float*)target.getBegin();

				for (uintreg i = 0u; i < m_data.getCount(); i += ChannelCount)
				{
					const uintreg targetIndex = ( i / 4u ) * 3u;

					pTargetData[ targetIndex + 0u ] = m_data[ i + 2u ];
					pTargetData[ targetIndex + 1u ] = m_data[ i + 1u ];
					pTargetData[ targetIndex + 2u ] = m_data[ i + 0u ];
				}
			}
			break;
		case PixelFormat_R32G32B32A32_Float:
			{
				target.create( m_width * m_height * 4u * sizeof( float ), TIKI_DEFAULT_ALIGNMENT, false );
				float* pTargetData = (float*)target.getBegin();

				for (uintreg i = 0u; i < m_data.getCount(); i += ChannelCount)
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

	bool HdrImage::loadPsdFile( const char* pFilename )
	{
		psd_context* pContext = nullptr;
		psd_status status = psd_image_load( &pContext, (psd_char*)pFilename );

		if( status != psd_status_done )
		{
			TIKI_TRACE_ERROR( "input file can't parse: %s\n", pFilename );
			return false;
		}

		status = psd_image_blend( pContext, 0, 0, pContext->width, pContext->height );
		if( status != psd_status_done )
		{
			TIKI_TRACE_ERROR( "psd blending failed for %s\n", pFilename );
			psd_image_free( pContext );
			return false;
		}

		m_gammaType = (pContext->color_mode == psd_color_mode_rgb ? GammaType_SRGB : GammaType_Linear);
		m_width = pContext->width;
		m_height = pContext->height;

		if( !m_data.create( m_width * m_height * ChannelCount, TIKI_DEFAULT_ALIGNMENT, false ) )
		{
			TIKI_TRACE_ERROR( "out of memory. can't store '%s' in memory!\n", pFilename );
			psd_image_free( pContext );
			return false;
		}

		const uint8* pPixelData = (const uint8*)pContext->blending_image_data;
		for( uintreg i = 0u; i < m_data.getCount(); ++i )
		{
			m_data[ i ] = (float)pPixelData[ i ] / 255.0f;
		}

		psd_image_free( pContext );
		return true;
	}

	bool HdrImage::loadPngFile( const char* pFilename )
	{
		int width;
		int height;
		int bpp;
		uint8* pPixelData = stbi_load( pFilename, &width, &height, &bpp, STBI_rgb_alpha );
		if( pPixelData == nullptr )
		{
			TIKI_TRACE_ERROR( "input file can't parse: %s\n", pFilename );
			return false;
		}

		m_gammaType = GammaType_Linear;
		m_width = width;
		m_height = height;

		if( !m_data.create( m_width * m_height * ChannelCount, TIKI_DEFAULT_ALIGNMENT, false ) )
		{
			TIKI_TRACE_ERROR( "out of memory. can't store '%s' in memory!\n", pFilename );
			stbi_image_free( pPixelData );
			return false;
		}

		for( uintreg i = 0u; i < m_data.getCount(); i += 4u )
		{
			m_data[ i + 0u ] = (float)pPixelData[ i + 2u ] / 255.0f;
			m_data[ i + 1u ] = (float)pPixelData[ i + 1u ] / 255.0f;
			m_data[ i + 2u ] = (float)pPixelData[ i + 0u ] / 255.0f;
			m_data[ i + 3u ] = (float)pPixelData[ i + 3u ] / 255.0f;
		}

		stbi_image_free( pPixelData );
		return true;
	}
}