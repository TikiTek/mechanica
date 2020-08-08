#pragma once

#include "tiki/converter_base/converter_base.hpp"

#include "tiki/tool_generic_data/generic_data_type_collection.hpp"

namespace tiki
{
	class Project;

	class GenericDataConverter : public ConverterBase
	{
	public:

										GenericDataConverter();
		virtual							~GenericDataConverter();

	protected:

		virtual uint32					getConverterRevision( crc32 typeCrc ) const TIKI_OVERRIDE_FINAL;
		virtual bool					canConvertType( crc32 typeCrc ) const TIKI_OVERRIDE_FINAL;

		virtual void					getInputExtensions( List< DynamicString >& extensions ) const TIKI_OVERRIDE_FINAL;
		virtual const char*				getOutputType() const TIKI_OVERRIDE_FINAL;

		virtual bool					initializeConverter( const ConversionContext& context ) TIKI_OVERRIDE_FINAL;
		virtual void					disposeConverter() TIKI_OVERRIDE_FINAL;

		virtual bool					startConversionJob( ConversionResult& result, const ConversionAsset& asset, const ConversionContext& context ) const TIKI_OVERRIDE_FINAL;

	private:

		typedef Map< crc32, const GenericDataType* > TypeCrcMap;

		GenericDataTypeCollection		m_typeCollection;

		TypeCrcMap						m_resourceTypeMap;
	};
}
