#pragma once
#ifndef TIKI_CONVERSION_ASSET_HPP_INCLUDED
#define TIKI_CONVERSION_ASSET_HPP_INCLUDED

#include "tiki/base/dynamic_string.hpp"
#include "tiki/base/path.hpp"
#include "tiki/converterbase/conversion_parameters.hpp"

namespace tiki
{
	struct ConversionAsset
	{
		uint					assetId;
		string					assetName;

		bool					isBuildRequired;

		Path					inputFilePath;
		crc32					typeCrc;

		ConversionParameters	parameters;
	};
}

#endif // TIKI_CONVERSION_ASSET_HPP_INCLUDED
