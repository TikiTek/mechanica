#pragma once
#ifndef TIKI_IASSETCONVERTER_HPP
#define TIKI_IASSETCONVERTER_HPP

#include "tiki/base/dynamic_string.hpp"
#include "tiki/base/library_types.hpp"
#include "tiki/base/types.hpp"
#include "tiki/container/array.hpp"

#if TIKI_ENABLED( TIKI_BUILD_LIBRARY )
#	define TIKI_ASSET_CONVERTER_METHOD	TIKI_LIBRARY_EXPORT_METHOD
#else
#	define TIKI_ASSET_CONVERTER_METHOD	TIKI_LIBRARY_IMPORT_METHOD
#endif

namespace tiki
{
	struct AssetConverterParamter
	{
		AssetConverterParamter()
		{
			waitForConversion			= false;
			forceRebuild				= false;
			rebuildOnMissingDatabase	= true;
		}

		string	sourcePath;
		string	outputPath;
		string	packageName;

		bool	waitForConversion;
		bool	forceRebuild;
		bool	rebuildOnMissingDatabase;
	};

	class IAssetConverter
	{
	public:

						IAssetConverter() {}
		virtual			~IAssetConverter() {}

		virtual bool	create( const AssetConverterParamter& parameters ) TIKI_PURE;
		virtual void	dispose() TIKI_PURE;

		virtual bool	convertAll() TIKI_PURE;

		virtual void	startWatch() TIKI_PURE;
		virtual void	stopWatch() TIKI_PURE;
		virtual bool	getChangedFiles( Array< string >& changedFiles ) TIKI_PURE;

		virtual void	lockConversion() TIKI_PURE;
		virtual void	unlockConversion() TIKI_PURE;
	};

	TIKI_ASSET_CONVERTER_METHOD IAssetConverter*	createAssetConverter();
	TIKI_ASSET_CONVERTER_METHOD void				disposeAssetConverter( IAssetConverter* pObject );
}

#endif // TIKI_IASSETCONVERTER_HPP
