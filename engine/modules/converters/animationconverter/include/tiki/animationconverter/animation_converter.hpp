#pragma once
#ifndef TIKI_ANIMATIONCONVERTER_HPP
#define TIKI_ANIMATIONCONVERTER_HPP

#include "tiki/converterbase/converterbase.hpp"

namespace tiki
{
	class AnimationConverter : public ConverterBase
	{
	public:

						AnimationConverter();
		virtual			~AnimationConverter();

	protected:

		virtual uint32	getConverterRevision( crc32 typeCrc ) const TIKI_OVERRIDE_FINAL;
		virtual bool	canConvertType( crc32 typeCrc ) const TIKI_OVERRIDE_FINAL;

		virtual void	getInputExtensions( List< string >& extensions ) const TIKI_OVERRIDE_FINAL;
		virtual crc32	getOutputType() const TIKI_OVERRIDE_FINAL;

		virtual bool	initializeConverter() TIKI_OVERRIDE_FINAL;
		virtual void	disposeConverter() TIKI_OVERRIDE_FINAL;

		virtual bool	startConversionJob( ConversionResult& result, const ConversionAsset& asset ) const TIKI_OVERRIDE_FINAL;
	};
}

#endif // TIKI_ANIMATIONCONVERTER_HPP
