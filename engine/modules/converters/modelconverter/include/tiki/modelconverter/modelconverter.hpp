#ifndef TIKI_MODELCONVERTER_HPP__
#define TIKI_MODELCONVERTER_HPP__

#include "tiki/converterbase/converterbase.hpp"

#include "tiki/converterbase/resourcewriter.hpp"

namespace tiki
{
	class ToolModelGeometrie;
	class ToolModelHierarchy;

	class ModelConverter : public ConverterBase
	{
	public:

								ModelConverter();
		virtual					~ModelConverter();

	protected:

		virtual uint32			getConverterRevision( crc32 typeCrc ) const TIKI_OVERRIDE;
		virtual bool			canConvertType( crc32 typeCrc ) const TIKI_OVERRIDE;

		virtual crc32			getOutputType() const TIKI_OVERRIDE;
		virtual void			getDependingType( List< crc32 >& types ) const TIKI_OVERRIDE;

		virtual bool			initializeConverter() TIKI_OVERRIDE;
		virtual void			disposeConverter() TIKI_OVERRIDE;

		virtual bool			startConversionJob( ConversionResult& result, const ConversionParameters& parameters ) const TIKI_OVERRIDE;

	private:

		ReferenceKey			writeHierarchy( ResourceWriter& fileWriter, const ToolModelHierarchy& hierarchy ) const;
		ReferenceKey			writeGeometry( ResourceWriter& fileWriter, const ToolModelGeometrie& geometry ) const;

	};
}

#endif // TIKI_MODELCONVERTER_HPP__
