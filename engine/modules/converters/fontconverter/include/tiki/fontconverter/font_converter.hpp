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

		virtual uint32	getConverterRevision( crc32 typeCrc ) const TIKI_OVERRIDE_FINAL;
		virtual bool	canConvertType( crc32 typeCrc ) const TIKI_OVERRIDE_FINAL;

		virtual void	getInputExtensions( List< string >& extensions ) const TIKI_OVERRIDE_FINAL;
		virtual crc32	getOutputType() const TIKI_OVERRIDE_FINAL;

		virtual bool	initializeConverter() TIKI_OVERRIDE_FINAL;
		virtual void	disposeConverter() TIKI_OVERRIDE_FINAL;

		virtual bool	startConversionJob( ConversionResult& result, const ConversionAsset& asset, const ConversionContext& context ) const TIKI_OVERRIDE_FINAL;
	};
}

#endif // TIKI_FONTCONVERTER_HPP__INCLUDED
