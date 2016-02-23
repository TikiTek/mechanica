-- config

local module = Module:new( "config" );

module.import_func = function()
	if use_mingw then
		buildoptions { "-std=c++11" }
	end
end

module:set_define( "TIKI_ON", "2" );
module:set_define( "TIKI_OFF", "1" );

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
module:set_flag( "OptimizeSpeed", "Release" );

module:set_define( "NDEBUG", nil, "Master" );
module:set_define( "TIKI_BUILD_DEBUG", "TIKI_OFF", "Master" );
module:set_define( "TIKI_BUILD_RELEASE", "TIKI_OFF", "Master" );
module:set_define( "TIKI_BUILD_MASTER", "TIKI_ON", "Master" );
module:set_flag( "Optimize", "Master" );
module:set_flag( "OptimizeSpeed", "Master" );

is_windows	= false;
is_linux	= false;
is_apple	= false;

use_msvc	= false;
use_mingw	= false;

use_d3d11	= false;
use_d3d12	= false;
use_opengl	= false;

use_sdl		= false;

if _ACTION == "vs2010" or _ACTION == "vs2012" or _ACTION == "vs2013" or _ACTION == "vs2015" then
	module:set_define( "WIN_NT" );
	module:set_define( "WIN32" );
	module:set_define( "_WIN32" );
	module:set_define( "_WIN64", nil, nil, "x64" );
	
	module:set_define( "_CRT_SECURE_NO_WARNINGS" );
	module:set_define( "_WINSOCK_DEPRECATED_NO_WARNINGS" );	
	module:set_define( "_HAS_EXCEPTIONS", "0" );	
	
	module:set_flag("MultiProcessorCompile");
	
	use_msvc	= true;
	use_d3d11	= true;
	--use_d3d12	= true;
	--use_opengl	= true;
	
	global_configuration.enable_unity_builds = true	
elseif _ACTION == "gmake" or _ACTION == "codeblocks" or _ACTION == "codelite" or _ACTION == "eclipse" then
	use_mingw	= true;
	use_opengl	= true;
	use_sdl		= true;
	
	if _ACTION == "codeblocks" then
	end
	
	--if _ACTION == "codelite" then
	--	include( "codelite" );
	--elseif _ACTION == "codeblocks" then
	--	include( "codeblocks" );
	--elseif _ACTION == "eclipse" then
	--	include( "eclipse" );
	--end
elseif _ACTION == "xcode" then
	use_opengl	= true;
else
	throw("Build action not supported.");
end

if os.get() == "windows" then
	is_windows = true;
elseif os.get() == "bsd" or os.get() == "linux" or os.get() == "solaris" then
	is_linux = true;
elseif os.get() == "macosx" then
	is_apple = true;
else
	throw("Operating System not supported.");
end

module:set_define( "TIKI_PLATFORM_WIN", iff( is_windows, "TIKI_ON", "TIKI_OFF" ) );
module:set_define( "TIKI_PLATFORM_LINUX", iff( is_linux, "TIKI_ON", "TIKI_OFF" ) );
module:set_define( "TIKI_PLATFORM_APPLE", iff( is_apple, "TIKI_ON", "TIKI_OFF" ) );

module:set_define( "TIKI_BUILD_32BIT", "TIKI_ON", nil, "x32" );
module:set_define( "TIKI_BUILD_64BIT", "TIKI_OFF", nil, "x32" );

module:set_define( "TIKI_BUILD_32BIT", "TIKI_OFF", nil, "x64" );
module:set_define( "TIKI_BUILD_64BIT", "TIKI_ON", nil, "x64" );

module:set_define( "TIKI_GRAPHICS_D3D11", iff( use_d3d11, "TIKI_ON", "TIKI_OFF" ) );
module:set_define( "TIKI_GRAPHICS_D3D12", iff( use_d3d12, "TIKI_ON", "TIKI_OFF" ) );
module:set_define( "TIKI_GRAPHICS_OPENGL4", iff( use_opengl, "TIKI_ON", "TIKI_OFF" ) );

module:set_define( "TIKI_SDL", iff( use_sdl, "TIKI_ON", "TIKI_OFF" ) );

module:set_define( "TIKI_BUILD_MSVC", iff( use_msvc, "TIKI_ON", "TIKI_OFF" ) );
module:set_define( "TIKI_BUILD_MINGW", iff( use_mingw, "TIKI_ON", "TIKI_OFF" ) );

--module:set_flag( "FatalWarnings" );
module:set_flag( "NoRTTI" );
module:set_flag( "NoExceptions" );
module:set_flag( "NoMinimalRebuild" );
