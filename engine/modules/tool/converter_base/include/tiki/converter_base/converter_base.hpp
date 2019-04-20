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

		void						create( const Path& outputPath, TaskSystem* pTaskSystem );
		void						dispose();

		void						convert( ConversionResult& result, const ConversionAsset& asset, const ConversionContext& context ) const;

	protected:

		void						openResourceWriter( ResourceWriter& writer, ConversionResult& result, const char* pFileName, const char* pExtension ) const;
		void						closeResourceWriter( ResourceWriter& writer ) const;

		TaskId						queueTask( TaskFunc pFunc, void* pData, TaskId dependingTaskId = InvalidTaskId ) const;
		void						waitForTask( TaskId taskId ) const;

		List< ResourceDefinition >	getResourceDefinitions( FlagMask8< ResourceDefinitionFeature > features ) const;

		virtual uint32				getConverterRevision( crc32 typeCrc ) const TIKI_PURE;
		virtual bool				canConvertType( crc32 typeCrc ) const TIKI_PURE;

		virtual void				getInputExtensions( List< string >& extensions ) const TIKI_PURE;
		virtual crc32				getOutputType() const TIKI_PURE;

		virtual bool				initializeConverter() TIKI_PURE;
		virtual void				disposeConverter() TIKI_PURE;

		virtual bool				startConversionJob( ConversionResult& result, const ConversionAsset& asset, const ConversionContext& context ) const TIKI_PURE;

	private:

		Path						m_outputPath;
		TaskSystem*					m_pTaskSystem;
	};
}