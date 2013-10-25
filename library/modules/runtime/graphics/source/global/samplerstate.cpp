

#include "tiki/base/types.hpp"

#if TIKI_ENABLED( TIKI_GRAPHICS_D3D11 )
#	include "../win_d3d11/samplerstate_d3d11.cpp"
#elif TIKI_ENABLED( TIKI_GRAPHICS_OPENGL4 )
#	include "../global_opengl4/samplerstate_opengl4.cpp"
#else
#	error Platform not supported
#endif