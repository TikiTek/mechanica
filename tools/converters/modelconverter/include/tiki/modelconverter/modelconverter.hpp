#ifndef TIKI_MODELCONVERTER_HPP__
#define TIKI_MODELCONVERTER_HPP__

#include "tiki/converterbase/converterbase.hpp"

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

		virtual size_t			getConverterRevision() const { return 29u; }

		virtual crc32			getInputType() const;
		virtual crc32			getOutputType() const;
		virtual void			getDependingType( List< crc32 >& types ) const;

		virtual bool			initializeConverter();
		virtual void			disposeConverter();

		virtual bool			startConversionJob( const ConversionParameters& params ) const;

	private:

		void					writeHierarchy( ResourceWriter& fileWriter, const ToolModelHierarchy& hierarchy ) const;
		void					writeGeometry( ResourceWriter& fileWriter, const ToolModelGeometrie& geometry ) const;

	};
}

#endif // TIKI_MODELCONVERTER_HPP__
