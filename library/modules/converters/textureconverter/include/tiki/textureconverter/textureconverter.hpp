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

		virtual size_t			getConverterRevision() const { return 9u; }

		virtual crc32			getInputType() const;
		virtual crc32			getOutputType() const;
		virtual void			getDependingType( List< crc32 >& types ) const;

		virtual bool			initializeConverter();
		virtual void			disposeConverter();

		virtual bool			startConversionJob( const ConversionParameters& params ) const;

	};
}

#endif // TIKI_TEXTURECONVERTER_HPP