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

module:set_define( "NDEBUG", nil, "Release" );
module:set_define( "TIKI_BUILD_DEBUG", "TIKI_OFF", "Release" );
module:set_define( "TIKI_BUILD_RELEASE", "TIKI_ON", "Release" );
module:set_define( "TIKI_BUILD_MASTER", "TIKI_OFF", "Release" );
module:set_flag( "Optimize", "Release" );

module:set_define( "NDEBUG", nil, "Master" );
module:set_define( "TIKI_BUILD_DEBUG", "TIKI_OFF", "Master" );
module:set_define( "TIKI_BUILD_RELEASE", "TIKI_OFF", "Master" );
module:set_define( "TIKI_BUILD_MASTER", "TIKI_ON", "Master" );
module:set_flag( "Optimize", "Master" );

if _ACTION == "vs2010" then
	module:set_define( "TIKI_PLATFORM_WIN", "TIKI_ON" );
	module:set_define( "TIKI_PLATFORM_LINUX", "TIKI_OFF" );
	module:set_define( "TIKI_PLATFORM_APPLE", "TIKI_OFF" );

	module:set_define( "WIN32" );
	module:set_define( "_WIN32" );

	module:set_define( "_WIN64", nil, "Debug", "x64" );
	module:set_define( "_WIN64", nil, "Release", "x64" );
	module:set_define( "_WIN64", nil, "Master", "x64" );

	is_windows = true;
	is_linux = false;
elseif _ACTION == "gmake" then
	module:set_define( "TIKI_PLATFORM_WIN", "TIKI_OFF" );
	module:set_define( "TIKI_PLATFORM_LINUX", "TIKI_ON" );
	module:set_define( "TIKI_PLATFORM_APPLE", "TIKI_OFF" );

	is_windows = false;
	is_linux = true;
end

module:set_define( "TIKI_BUILD_32BIT", "TIKI_ON", "Debug", "x32" );
module:set_define( "TIKI_BUILD_32BIT", "TIKI_ON", "Release", "x32" );
module:set_define( "TIKI_BUILD_32BIT", "TIKI_ON", "Master", "x32" );
module:set_define( "TIKI_BUILD_64BIT", "TIKI_OFF", "Debug", "x32" );
module:set_define( "TIKI_BUILD_64BIT", "TIKI_OFF", "Release", "x32" );
module:set_define( "TIKI_BUILD_64BIT", "TIKI_OFF", "Master", "x32" );

module:set_define( "TIKI_BUILD_32BIT", "TIKI_OFF", "Debug", "x64" );
module:set_define( "TIKI_BUILD_32BIT", "TIKI_OFF", "Release", "x64" );
module:set_define( "TIKI_BUILD_32BIT", "TIKI_OFF", "Master", "x64" );
module:set_define( "TIKI_BUILD_64BIT", "TIKI_ON", "Debug", "x64" );
module:set_define( "TIKI_BUILD_64BIT", "TIKI_ON", "Release", "x64" );
module:set_define( "TIKI_BUILD_64BIT", "TIKI_ON", "Master", "x64" );

module:set_define( "TIKI_BUILD_TOOLS", "TIKI_OFF" );