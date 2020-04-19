#pragma once

#include "tiki/converter_base/converter_base.hpp"

namespace tiki
{
	class AnimationConverter : public ConverterBase
	{
	public:

							AnimationConverter();
		virtual				~AnimationConverter();

	protected:

		virtual uint32		getConverterRevision( crc32 typeCrc ) const TIKI_OVERRIDE_FINAL;
		virtual bool		canConvertType( crc32 typeCrc ) const TIKI_OVERRIDE_FINAL;

		virtual void		getInputExtensions( List< DynamicString >& extensions ) const TIKI_OVERRIDE_FINAL;
		virtual const char*	getOutputType() const TIKI_OVERRIDE_FINAL;

		virtual bool		initializeConverter( const ConversionContext& context ) TIKI_OVERRIDE_FINAL;
		virtual void		disposeConverter() TIKI_OVERRIDE_FINAL;

		virtual bool		startConversionJob( ConversionResult& result, const ConversionAsset& asset, const ConversionContext& context ) const TIKI_OVERRIDE_FINAL;
	};
}
