#pragma once
#ifndef TIKI_GENERICDATACONVERTER_HPP_INCLUDED__
#define TIKI_GENERICDATACONVERTER_HPP_INCLUDED__

#include "tiki/converterbase/converterbase.hpp"

#include "tiki/toolgenericdata/generic_data_type_collection.hpp"

namespace tiki
{
	class GenericDataConverter : public ConverterBase
	{
	public:

									GenericDataConverter();
		virtual						~GenericDataConverter();

	protected:

		virtual uint32				getConverterRevision( crc32 typeCrc ) const TIKI_OVERRIDE_FINAL;
		virtual bool				canConvertType( crc32 typeCrc ) const TIKI_OVERRIDE_FINAL;

		virtual crc32				getOutputType() const TIKI_OVERRIDE_FINAL;
		virtual void				getDependingType( List< crc32 >& types ) const TIKI_OVERRIDE_FINAL;

		virtual bool				initializeConverter() TIKI_OVERRIDE_FINAL;
		virtual void				disposeConverter() TIKI_OVERRIDE_FINAL;

		virtual bool				startConversionJob( ConversionResult& result, const ConversionParameters& parameters ) const TIKI_OVERRIDE_FINAL;

	private:

		typedef Map< crc32, const GenericDataType* > TypeCrcMap;

		GenericDataTypeCollection	m_collection;

		TypeCrcMap					m_resourceTypeMap;

	};
}

#endif // TIKI_GENERICDATACONVERTER_HPP_INCLUDED__
