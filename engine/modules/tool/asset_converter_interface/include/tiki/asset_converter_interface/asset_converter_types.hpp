#pragma once

#include "tiki/base/types.hpp"

#if TIKI_ENABLED( TIKI_BUILD_LIBRARY )
#	define TIKI_ASSET_CONVERTER_METHOD	TIKI_LIBRARY_EXPORT_METHOD
#else
#	define TIKI_ASSET_CONVERTER_METHOD	TIKI_LIBRARY_IMPORT_METHOD
#endif