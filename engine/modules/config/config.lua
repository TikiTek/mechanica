Compilers = {
	None = 0,
	MSVC = 1,
	GCC = 2,
	Clang = 3
}

local module = Module:new();

--module.import_func = function()
--	if use_mingw then
--		buildoptions { "-std=c++11" }
--	end
--end

module:set_define( "TIKI_ON", "2" );
module:set_define( "TIKI_OFF", "1" );

module:set_define( "TIKI_BUILD_DEBUG", "TIKI_ON", "Debug" );
module:set_define( "TIKI_BUILD_RELEASE", "TIKI_OFF", "Debug" );
module:set_define( "TIKI_BUILD_MASTER", "TIKI_OFF", "Debug" );

module:set_define( "TIKI_BUILD_DEBUG", "TIKI_OFF", "Release" );
module:set_define( "TIKI_BUILD_RELEASE", "TIKI_ON", "Release" );
module:set_define( "TIKI_BUILD_MASTER", "TIKI_OFF", "Release" );

module:set_define( "TIKI_BUILD_DEBUG", "TIKI_OFF", "Master" );
module:set_define( "TIKI_BUILD_RELEASE", "TIKI_OFF", "Master" );
module:set_define( "TIKI_BUILD_MASTER", "TIKI_ON", "Master" );

local action = _ACTION;
if action == "targets" then
	action = _OPTIONS[ "targets_action" ]
	if not action then
		throw("No 'targets_action' option specified.");
	end
end

compiler = Compilers.None
if action == "vs2015" or action == "vs2017" or action == "vs2019" then
	--module:set_define( "WIN_NT" );
	--module:set_define( "WIN32" );
	--module:set_define( "_WIN32" );
	--module:set_define( "_WIN64", nil, nil, "x64" );
	
	module:set_define( "_CRT_SECURE_NO_WARNINGS" );
	--module:set_define( "_WINSOCK_DEPRECATED_NO_WARNINGS" );	
	--module:set_define( "_HAS_EXCEPTIONS", "0" );	
	
	compiler = Compilers.MSVC;
elseif action == "gmake" or action == "codeblocks" or action == "codelite" or action == "eclipse" then
	compiler = Compilers.GCC
else
	throw("Build action '" .. action .. "' not supported.");
end

module:set_define( "TIKI_PLATFORM_WIN", iff( tiki.platform == Platforms.Windows, "TIKI_ON", "TIKI_OFF" ) );
module:set_define( "TIKI_PLATFORM_LINUX", iff( tiki.platform == Platforms.Linux, "TIKI_ON", "TIKI_OFF" ) );

module:set_define( "TIKI_BUILD_32BIT", "TIKI_ON", nil, "x32" );
module:set_define( "TIKI_BUILD_64BIT", "TIKI_OFF", nil, "x32" );

module:set_define( "TIKI_BUILD_32BIT", "TIKI_OFF", nil, "x64" );
module:set_define( "TIKI_BUILD_64BIT", "TIKI_ON", nil, "x64" );

module:set_define( "TIKI_BUILD_MSVC", iff( compiler == Compilers.MSVC, "TIKI_ON", "TIKI_OFF" ) );
module:set_define( "TIKI_BUILD_GCC", iff( compiler == Compilers.GCC, "TIKI_ON", "TIKI_OFF" ) );
module:set_define( "TIKI_BUILD_CLANG", iff( compiler == Compilers.Clang, "TIKI_ON", "TIKI_OFF" ) );

--module:set_flag( "FatalWarnings" );
