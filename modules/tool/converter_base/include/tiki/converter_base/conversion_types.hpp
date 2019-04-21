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

		bool					isAlradyBuilt;

		Path					inputFilePath;
		string					type;

		ConversionParameters	parameters;
	};

	struct ConversionContext
	{
		const Project*			pProject;
	};
}
