#pragma once

#include "tiki/converter_base/converter_base.hpp"

namespace tiki
{
	class TextureConverter : public ConverterBase
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( TextureConverter );

	protected:

		virtual uint32			getConverterRevision( crc32 typeCrc ) const TIKI_OVERRIDE_FINAL;
		virtual bool			canConvertType( crc32 typeCrc ) const TIKI_OVERRIDE_FINAL;

		virtual void			getInputExtensions( List< string >& extensions ) const TIKI_OVERRIDE_FINAL;
		virtual crc32			getOutputType() const TIKI_OVERRIDE_FINAL;

		virtual bool			initializeConverter() TIKI_OVERRIDE_FINAL;
		virtual void			disposeConverter() TIKI_OVERRIDE_FINAL;

		virtual bool			startConversionJob( ConversionResult& result, const ConversionAsset& asset, const ConversionContext& context ) const TIKI_OVERRIDE_FINAL;

	};
}
