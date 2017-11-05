#ifndef TIKI_MODELCONVERTER_HPP__
#define TIKI_MODELCONVERTER_HPP__

#include "tiki/converterbase/converterbase.hpp"

#include "tiki/converterbase/resource_writer.hpp"

namespace tiki
{
	class ToolModelGeometrie;
	class ToolModelHierarchy;

	class ModelConverter : public ConverterBase
	{
	public:

						ModelConverter();
		virtual			~ModelConverter();

	protected:

		virtual uint32	getConverterRevision( crc32 typeCrc ) const TIKI_OVERRIDE_FINAL;
		virtual bool	canConvertType( crc32 typeCrc ) const TIKI_OVERRIDE_FINAL;

		virtual void	getInputExtensions( List< string >& extensions ) const TIKI_OVERRIDE_FINAL;
		virtual crc32	getOutputType() const TIKI_OVERRIDE_FINAL;

		virtual bool	initializeConverter() TIKI_OVERRIDE_FINAL;
		virtual void	disposeConverter() TIKI_OVERRIDE_FINAL;

		virtual bool	startConversionJob( ConversionResult& result, const ConversionAsset& asset ) const TIKI_OVERRIDE_FINAL;

	private:

		ReferenceKey	writeHierarchy( ResourceWriter& resourceWriter, const ToolModelHierarchy& hierarchy ) const;
		ReferenceKey	writeGeometry( ResourceWriter& resourceWriter, const ToolModelGeometrie& geometry ) const;

	};
}

#endif // TIKI_MODELCONVERTER_HPP__
