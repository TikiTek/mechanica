#pragma once
#ifndef TIKI_FONTCONVERTER_HPP__INCLUDED
#define TIKI_FONTCONVERTER_HPP__INCLUDED

#include "tiki/converterbase/converterbase.hpp"

namespace tiki
{
	class FontConverter : public ConverterBase
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( FontConverter );

	protected:

		virtual uint16			getConverterRevision() const TIKI_OVERRIDE;

		virtual bool			canConvertType( crc32 typeCrc ) const TIKI_OVERRIDE;

		virtual crc32			getOutputType() const TIKI_OVERRIDE;
		virtual void			getDependingType( List< crc32 >& types ) const TIKI_OVERRIDE;

		virtual bool			initializeConverter() TIKI_OVERRIDE;
		virtual void			disposeConverter() TIKI_OVERRIDE;

		virtual bool			startConversionJob( ConversionResult& result, const ConversionParameters& parameters ) const TIKI_OVERRIDE;

	};
}

#endif // TIKI_FONTCONVERTER_HPP__INCLUDED
