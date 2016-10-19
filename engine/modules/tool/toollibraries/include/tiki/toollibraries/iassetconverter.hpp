#pragma once
#ifndef TIKI_IASSETCONVERTER_HPP
#define TIKI_IASSETCONVERTER_HPP

#include "tiki/container/array.hpp"
#include "tiki/base/basicstring.hpp"
#include "tiki/base/types.hpp"
#include "tiki/toollibraries/librarybase.hpp"

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

	TIKI_LIBRARY_METHOD IAssetConverter*	createAssetConverter();
	TIKI_LIBRARY_METHOD void				disposeAssetConverter( IAssetConverter* pObject );
}

#endif // TIKI_IASSETCONVERTER_HPP
