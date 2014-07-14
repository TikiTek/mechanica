-- config

local module = Module:new( "config" );

module:set_define( "TIKI_ON", "2-" );
module:set_define( "TIKI_OFF", "1-" );
module:set_define( "TIKI_CURRENT_MODULE", "NOT_IMPLEMENTED" );

module:set_define( "DEBUG", nil, "Debug" );
module:set_define( "TIKI_BUILD_DEBUG", "TIKI_ON", "Debug" );
module:set_define( "TIKI_BUILD_RELEASE", "TIKI_OFF", "Debug" );
module:set_define( "TIKI_BUILD_MASTER", "TIKI_OFF", "Debug" );
module:set_flag( "Symbols", "Debug" );
--module:set_flag( "FatalWarnings", "Debug" );

module:set_define( "NDEBUG", nil, "Release" );
module:set_define( "TIKI_BUILD_DEBUG", "TIKI_OFF", "Release" );
module:set_define( "TIKI_BUILD_RELEASE", "TIKI_ON", "Release" );
module:set_define( "TIKI_BUILD_MASTER", "TIKI_OFF", "Release" );
module:set_flag( "Optimize", "Release" );
module:set_flag( "NoRTTI", "Release" );
module:set_flag( "OptimizeSpeed", "Release" );
--module:set_flag( "FatalWarnings", "Release" );

module:set_define( "NDEBUG", nil, "Master" );
module:set_define( "TIKI_BUILD_DEBUG", "TIKI_OFF", "Master" );
module:set_define( "TIKI_BUILD_RELEASE", "TIKI_OFF", "Master" );
module:set_define( "TIKI_BUILD_MASTER", "TIKI_ON", "Master" );
module:set_flag( "Optimize", "Master" );
module:set_flag( "NoRTTI", "Master" );
module:set_flag( "OptimizeSpeed", "Master" );
--module:set_flag( "FatalWarnings", "Master" );

is_windows	= false;
is_linux	= false;
is_apple	= false;

use_d3d11	= false;
use_opengl	= false;

if _ACTION == "vs2010" or _ACTION == "vs2012" then
	module:set_define( "WIN_NT" );
	module:set_define( "WIN32" );
	module:set_define( "_WIN32" );
	module:set_define( "_WIN64", nil, nil, "x64" );

	is_windows	= true;	
	use_d3d11	= true;
elseif _ACTION == "gmake" then
	is_linux	= true;
	use_opengl	= true;
elseif _ACTION == "xcode" then
	is_apple	= true;
	use_opengl	= true;
end

module:set_define( "TIKI_PLATFORM_WIN", iff( is_windows, "TIKI_ON", "TIKI_OFF" ) );
module:set_define( "TIKI_PLATFORM_LINUX", iff( is_linux, "TIKI_ON", "TIKI_OFF" ) );
module:set_define( "TIKI_PLATFORM_APPLE", iff( is_apple, "TIKI_ON", "TIKI_OFF" ) );

module:set_define( "TIKI_BUILD_32BIT", "TIKI_ON", nil, "x32" );
module:set_define( "TIKI_BUILD_64BIT", "TIKI_OFF", nil, "x32" );

module:set_define( "TIKI_BUILD_32BIT", "TIKI_OFF", nil, "x64" );
module:set_define( "TIKI_BUILD_64BIT", "TIKI_ON", nil, "x64" );

module:set_define( "TIKI_GRAPHICS_D3D11", iff( use_d3d11, "TIKI_ON", "TIKI_OFF" ) );
module:set_define( "TIKI_GRAPHICS_OPENGL4", iff( use_opengl, "TIKI_ON", "TIKI_OFF" ) );
