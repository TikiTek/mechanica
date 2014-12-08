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

		virtual crc32			getInputType() const = 0;
		virtual crc32			getOutputType() const = 0;
		virtual void			getDependingType( List< crc32 >& types ) const = 0;
		virtual uint16			getConverterRevision() const = 0;

		virtual bool			initializeConverter() = 0;
		virtual void			disposeConverter() = 0;

		virtual bool			startConversionJob( ConversionResult& result, const ConversionParameters& parameters ) const = 0;

	private:

		ConverterManager*			m_pManager;

	};
}

#endif // TIKI_CONVERTERBASE_HPP
