-- library/modules/runtime/input

local module = Module:new( "input" );

module:add_files( "include/**/*.*" );
module:add_include_dir( "include" );

module:add_dependency( "base" );

module:add_library_file( "dxguid" );
module:add_library_file( "dinput8" );
module:add_library_file( "xinput" );

if is_windows then
	module:add_files( "source/win/*.*" );
elseif is_linux then
	module:add_files( "source/linux/*.*" );
else
	throw "Platform not implemented"
end