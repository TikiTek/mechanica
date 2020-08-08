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
		DynamicString			assetName;

		bool					isAlradyBuilt;

		Path					inputFilePath;
		DynamicString			type;

		ConversionParameters	parameters;
	};

	struct ConversionContext
	{
		const Project*			pProject;
	};
}
