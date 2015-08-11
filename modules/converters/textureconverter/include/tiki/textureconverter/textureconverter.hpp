#pragma once
#ifndef TIKI_TEXTURECONVERTER_HPP
#define TIKI_TEXTURECONVERTER_HPP

#include "tiki/converterbase/converterbase.hpp"

namespace tiki
{
	class TextureConverter : public ConverterBase
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( TextureConverter );

	protected:

		virtual uint32			getConverterRevision( crc32 typeCrc ) const TIKI_OVERRIDE;
		virtual bool			canConvertType( crc32 typeCrc ) const TIKI_OVERRIDE;

		virtual crc32			getOutputType() const TIKI_OVERRIDE;
		virtual void			getDependingType( List< crc32 >& types ) const TIKI_OVERRIDE;

		virtual bool			initializeConverter() TIKI_OVERRIDE;
		virtual void			disposeConverter() TIKI_OVERRIDE;

		virtual bool			startConversionJob( ConversionResult& result, const ConversionParameters& params ) const TIKI_OVERRIDE;

	};
}

#endif // TIKI_TEXTURECONVERTER_HPP
