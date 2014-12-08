#ifndef TIKI_MATERIALCONVERTER_HPP__
#define TIKI_MATERIALCONVERTER_HPP__

#include "tiki/converterbase/converterbase.hpp"

namespace tiki
{
	class MaterialConverter : public ConverterBase
	{
	public:

						MaterialConverter();
		virtual			~MaterialConverter();

	protected:

		virtual uint16	getConverterRevision() const TIKI_OVERRIDE;

		virtual crc32	getInputType() const TIKI_OVERRIDE;
		virtual crc32	getOutputType() const TIKI_OVERRIDE;
		virtual void	getDependingType( List< crc32 >& types ) const TIKI_OVERRIDE;

		virtual bool	initializeConverter() TIKI_OVERRIDE;
		virtual void	disposeConverter() TIKI_OVERRIDE;

		virtual bool	startConversionJob( ConversionResult& result, const ConversionParameters& parameters ) const TIKI_OVERRIDE;

	};
}

#endif // TIKI_MATERIALCONVERTER_HPP__
