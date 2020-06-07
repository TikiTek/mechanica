#pragma once

#include "tiki/base/dynamic_string.hpp"
#include "tiki/base/path.hpp"
#include "tiki/base/platform.hpp"
#include "tiki/base/types.hpp"
#include "tiki/container/list.hpp"
#include "tiki/resource/resource_definition.hpp"
#include "tiki/task_system/task_system.hpp"

namespace tiki
{
	class AssetConverter;
	class ConversionResult;
	class ResourceDefinition;
	class ResourceWriter;
	struct ConversionAsset;
	struct ConversionContext;

	class ConverterBase
	{
		friend class AssetConverter;

	public:

									ConverterBase();
		virtual						~ConverterBase();

		void						create( const Path& outputPath, TaskSystem* pTaskSystem, const ConversionContext& context );
		void						dispose();

		void						convert( ConversionResult& result, const ConversionAsset& asset, const ConversionContext& context ) const;

	protected:

		void						openResourceWriter( ResourceWriter& writer, ConversionResult& result, const char* pBasename, const char* pExtension ) const;
		void						closeResourceWriter( ResourceWriter& writer ) const;

		TaskId						queueTask( TaskFunc pFunc, void* pData, TaskId dependingTaskId = InvalidTaskId ) const;
		void						waitForTask( TaskId taskId ) const;

		List< ResourceDefinition >	getResourceDefinitions( FlagMask8< ResourceDefinitionFeature > features ) const;

		virtual uint32				getConverterRevision( crc32 typeCrc ) const = 0;
		virtual bool				canConvertType( crc32 typeCrc ) const = 0;

		virtual void				getInputExtensions( List< DynamicString >& extensions ) const = 0;
		virtual const char*			getOutputType() const = 0;

		virtual bool				initializeConverter( const ConversionContext& context ) = 0;
		virtual void				disposeConverter() = 0;

		virtual bool				startConversionJob( ConversionResult& result, const ConversionAsset& asset, const ConversionContext& context ) const = 0;

	private:

		Path						m_outputPath;
		TaskSystem*					m_pTaskSystem;
	};
}
