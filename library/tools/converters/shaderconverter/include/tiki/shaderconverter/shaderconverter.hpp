#pragma once
#ifndef TIKI_SHADERCONVERTER_HPP
#define TIKI_SHADERCONVERTER_HPP

#include "tiki/converterbase/converterbase.hpp"
//#include "tiki/threading/mutex.hpp"

namespace tiki
{
	class ShaderConverter : public ConverterBase
	{
	public:

		virtual					~ShaderConverter() {}

	protected:

		virtual size_t			getConverterRevision() const { return 1u; }

		virtual crc32			getInputType() const;
		virtual crc32			getOutputType() const;
		virtual void			getDependingType( List< crc32 >& types ) const;

		virtual bool			initializeConverter();
		virtual void			disposeConverter();

		virtual bool			startConversionJob( const ConversionParameters& params ) const;

	private:

		//Mutex					m_mutex;

	};
}

#endif // TIKI_SHADERCONVERTER_HPP
