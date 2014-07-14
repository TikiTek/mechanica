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

		virtual uint16			getConverterRevision() const { return 1u; }

		virtual crc32			getInputType() const;
		virtual crc32			getOutputType() const;
		virtual void			getDependingType( List< crc32 >& types ) const;

		virtual bool			initializeConverter();
		virtual void			disposeConverter();

		virtual bool			startConversionJob( const ConversionParameters& params ) const;

	private:

		ReferenceKey			writeHierarchy( ResourceWriter& fileWriter, const ToolModelHierarchy& hierarchy ) const;
		ReferenceKey			writeGeometry( ResourceWriter& fileWriter, const ToolModelGeometrie& geometry ) const;

	};
}

#endif // TIKI_MODELCONVERTER_HPP__
