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

		virtual uint	getConverterRevision() const { return 1u; }

		virtual crc32	getInputType() const;
		virtual crc32	getOutputType() const;
		virtual void	getDependingType( List< crc32 >& types ) const;

		virtual bool	initializeConverter();
		virtual void	disposeConverter();

		virtual bool	startConversionJob( const ConversionParameters& params ) const;
	};
}

#endif // TIKI_MATERIALCONVERTER_HPP__
