#pragma once
#ifndef TIKI_CONVERTERBASE_HPP
#define TIKI_CONVERTERBASE_HPP

#include "tiki/base/platform.hpp"
#include "tiki/base/string.hpp"
#include "tiki/base/types.hpp"
#include "tiki/converterbase/conversionparameters.hpp"
#include "tiki/tasksystem/tasksystem.hpp"
#include "tiki/toolbase/list.hpp"

namespace tiki
{
	class ConversionResult;
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

		void					convert( ConversionResult& result, const ConversionParameters& params ) const;

		ConverterManager*		getManager() { return m_pManager; }

	protected:

		void					openResourceWriter( ResourceWriter& writer, ConversionResult& result, const string& fileName, const string& extension, PlatformType platform ) const;
		void					closeResourceWriter( ResourceWriter& writer ) const;

		TaskId					queueTask( TaskFunc pFunc, void* pData, TaskId dependingTaskId = InvalidTaskId ) const;
		void					waitForTask( TaskId taskId ) const;

		virtual uint32			getConverterRevision( crc32 typeCrc ) const TIKI_PURE;
		virtual bool			canConvertType( crc32 typeCrc ) const TIKI_PURE;

		virtual crc32			getOutputType() const TIKI_PURE;
		virtual void			getDependingType( List< crc32 >& types ) const TIKI_PURE;

		virtual bool			initializeConverter() TIKI_PURE;
		virtual void			disposeConverter() TIKI_PURE;

		virtual bool			startConversionJob( ConversionResult& result, const ConversionParameters& parameters ) const TIKI_PURE;

	private:

		ConverterManager*		m_pManager;

	};
}

#endif // TIKI_CONVERTERBASE_HPP
