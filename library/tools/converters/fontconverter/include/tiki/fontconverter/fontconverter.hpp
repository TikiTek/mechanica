#pragma once
#ifndef TIKI_FONTCONVERTER_HPP__INCLUDED
#define TIKI_FONTCONVERTER_HPP__INCLUDED

#include "tiki/converterbase/converterbase.hpp"

#include <time.h>

namespace tiki
{
	class FontConverter : public ConverterBase
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( FontConverter );

	protected:

		virtual size_t			getConverterRevision() const { return 1u; }

		virtual crc32			getInputType() const;
		virtual crc32			getOutputType() const;
		virtual void			getDependingType( List< crc32 >& types ) const;

		virtual bool			initializeConverter();
		virtual void			disposeConverter();

		virtual bool			startConversionJob( const ConversionParameters& params ) const;

	};
}

#endif // TIKI_FONTCONVERTER_HPP__INCLUDED
