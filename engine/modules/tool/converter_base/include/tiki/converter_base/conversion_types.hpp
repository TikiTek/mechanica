#pragma once

#include "tiki/base/dynamic_string.hpp"
#include "tiki/base/path.hpp"
#include "tiki/converter_base/conversion_parameters.hpp"

namespace tiki
{
	class Project;

	struct ConversionAsset
	{
		uint					assetId;
		string					assetName;

		bool					isBuildRequired;

		Path					inputFilePath;
		crc32					typeCrc;

		ConversionParameters	parameters;
	};

	struct ConversionContext
	{
		const Project*			pProject;
	};
}
