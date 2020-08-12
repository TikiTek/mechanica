#pragma once
#ifndef TIKI_GENERICDATACONVERTER_HPP_INCLUDED__
#define TIKI_GENERICDATACONVERTER_HPP_INCLUDED__

#include "tiki/converterbase/converterbase.hpp"

#include "tiki/toolgenericdata/genericdatatypecollection.hpp"

namespace tiki
{
	class GenericDataConverter : public ConverterBase
	{
	public:

									GenericDataConverter();
		virtual						~GenericDataConverter();

	protected:

		virtual uint32				getConverterRevision( crc32 typeCrc ) const TIKI_OVERRIDE;
		virtual bool				canConvertType( crc32 typeCrc ) const TIKI_OVERRIDE;

		virtual crc32				getOutputType() const TIKI_OVERRIDE;
		virtual void				getDependingType( List< crc32 >& types ) const TIKI_OVERRIDE;

		virtual bool				initializeConverter() TIKI_OVERRIDE;
		virtual void				disposeConverter() TIKI_OVERRIDE;

		virtual bool				startConversionJob( ConversionResult& result, const ConversionParameters& parameters ) const TIKI_OVERRIDE;

	private:

		typedef Map< crc32, const GenericDataType* > TypeCrcMap;

		GenericDataTypeCollection	m_collection;

		TypeCrcMap					m_resourceTypeMap;

	};
}

#endif // TIKI_GENERICDATACONVERTER_HPP_INCLUDED__
