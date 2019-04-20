#pragma once

#include "tiki/converter_base/converter_base.hpp"

#include "tiki/converter_base/resource_writer.hpp"

namespace tiki
{
	class ToolModelGeometrie;
	class ToolModelHierarchy;

	class ModelConverter : public ConverterBase
	{
	public:

							ModelConverter();
		virtual				~ModelConverter();

	protected:

		virtual uint32		getConverterRevision( crc32 typeCrc ) const TIKI_OVERRIDE_FINAL;
		virtual bool		canConvertType( crc32 typeCrc ) const TIKI_OVERRIDE_FINAL;

		virtual void		getInputExtensions( List< string >& extensions ) const TIKI_OVERRIDE_FINAL;
		virtual const char*	getOutputType() const TIKI_OVERRIDE_FINAL;

		virtual bool		initializeConverter() TIKI_OVERRIDE_FINAL;
		virtual void		disposeConverter() TIKI_OVERRIDE_FINAL;

		virtual bool		startConversionJob( ConversionResult& result, const ConversionAsset& asset, const ConversionContext& context ) const TIKI_OVERRIDE_FINAL;

	private:

		ReferenceKey		writeHierarchy( ResourceWriter& resourceWriter, const ToolModelHierarchy& hierarchy ) const;
		ReferenceKey		writeGeometry( ResourceWriter& resourceWriter, const ToolModelGeometrie& geometry ) const;
	};
}
