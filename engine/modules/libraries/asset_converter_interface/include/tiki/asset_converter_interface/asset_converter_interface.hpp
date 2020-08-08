#pragma once
#ifndef TIKI_IASSETCONVERTER_HPP
#define TIKI_IASSETCONVERTER_HPP

#include "tiki/asset_converter_interface/asset_converter_types.hpp"
#include "tiki/base/dynamic_string.hpp"
#include "tiki/base/library_types.hpp"
#include "tiki/container/array.hpp"

namespace tiki
{
	class Project;
	class AssetConverterStatus;

	struct AssetConverterParamter
	{
		AssetConverterParamter()
		{
			pProject					= nullptr;
			waitForConversion			= false;
			forceRebuild				= false;
			rebuildOnMissingDatabase	= true;
		}

		const Project*	pProject;

		bool			waitForConversion;
		bool			forceRebuild;
		bool			rebuildOnMissingDatabase;
	};

	class AssetConverterInterface
	{
	public:

						AssetConverterInterface() {}
		virtual			~AssetConverterInterface() {}

		virtual void	queueAll() = 0;

		virtual void	startWatch() = 0;
		virtual void	stopWatch() = 0;

		virtual bool	isConvertionRunning() = 0;
		virtual bool	popFinishConversion() = 0;
		virtual bool	wasLastBuildSuccessful() = 0;
		virtual bool	getChangedFiles( Array< DynamicString >& changedFiles ) = 0;
	};

	TIKI_ASSET_CONVERTER_METHOD AssetConverterInterface*	createAssetConverter( const AssetConverterParamter& parameters );
	TIKI_ASSET_CONVERTER_METHOD void						disposeAssetConverter( AssetConverterInterface* pAssetConverterInterface );
}

#endif // TIKI_IASSETCONVERTER_HPP
