#pragma once
#ifndef TIKI_CONVERTERBASE_HPP
#define TIKI_CONVERTERBASE_HPP

#include "tiki/base/platform.hpp"
#include "tiki/base/string.hpp"
#include "tiki/base/types.hpp"
#include "tiki/toolbase/list.hpp"

namespace tiki
{
	class ConverterManager;
	class ResourceWriter;
	struct ConversionParameters;

	class ConverterBase
	{
		friend class ConverterManager;

	public:

								ConverterBase();
		virtual					~ConverterBase();

		void					create( ConverterManager* pManager );
		void					dispose();

		void					convert( const ConversionParameters& params ) const;

		const ConverterManager*	getManager() const { return m_pManager; }

	protected:

		void					openResourceWriter( ResourceWriter& writer, const string& fileName, const string& extension, PlatformType platform ) const;
		void					closeResourceWriter( ResourceWriter& writer ) const;

		virtual crc32			getInputType() const = 0;
		virtual crc32			getOutputType() const = 0;
		virtual void			getDependingType( List< crc32 >& types ) const = 0;
		virtual size_t			getConverterRevision() const = 0;

		virtual bool			initializeConverter() = 0;
		virtual void			disposeConverter() = 0;

		virtual bool			startConversionJob( const ConversionParameters& params ) const = 0;

	private:

		ConverterManager*		m_pManager;

	};
}

#endif // TIKI_CONVERTERBASE_HPP