-- library/modules/runtime/input

local module = Module:new( "input" );

module:add_files( "include/**/*.*" );
module:add_include_dir( "include" );
module:add_files( "input.lua" );
module:add_files( "source/*.*" );

if use_sdl then
	module:add_files( "source/sdl/*.*" );
	module:add_dependency( "sdl" );
elseif use_msvc then
	module:add_files( "source/dinput/*.*" );
else
	throw "Platform not implemented"
end

module:add_dependency( "base" );

module:add_library_dir( "$(DXSDK_DIR)Lib/x86", nil, "x32" );
module:add_library_dir( "$(DXSDK_DIR)Lib/x64", nil, "x64" );

module:add_library_file( "dxguid" );
module:add_library_file( "dinput8" );
module:add_library_file( "xinput" );
