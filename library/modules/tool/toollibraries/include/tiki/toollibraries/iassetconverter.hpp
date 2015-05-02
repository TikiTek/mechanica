#pragma once
#ifndef TIKI_IASSETCONVERTER_HPP
#define TIKI_IASSETCONVERTER_HPP

#include "tiki/base/array.hpp"
#include "tiki/base/basicstring.hpp"
#include "tiki/base/types.hpp"
#include "tiki/toollibraries/librarybase.hpp"

namespace tiki
{
	struct AssetConverterParamter
	{
		AssetConverterParamter()
		{
			forceRebuild				= false;
			rebuildOnMissingDatabase	= true;
		}

		string	sourcePath;
		string	outputPath;

		bool	forceRebuild;
		bool	rebuildOnMissingDatabase;
	};

	class IAssetConverter
	{
	public:

						IAssetConverter() {}
		virtual			~IAssetConverter() {}

		virtual void	create( const AssetConverterParamter& parameters ) = 0;
		virtual void	dispose() = 0;

		virtual bool	convertAll() = 0;

		virtual void	startWatch() = 0;
		virtual void	stopWatch() = 0;
		virtual bool	getChangedFiles( Array< string >& changedFiles ) = 0;

		virtual void	lockConversion() = 0;
		virtual void	unlockConversion() = 0;
		
	};

	TIKI_LIBRARY_METHOD IAssetConverter*	createAssetConverter();
	TIKI_LIBRARY_METHOD void				disposeAssetConverter( IAssetConverter* pObject );
}

#endif // TIKI_IASSETCONVERTER_HPP