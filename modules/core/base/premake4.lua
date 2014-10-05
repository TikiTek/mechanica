-- library/modules/core/base

local module = Module:new( "base" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );

if use_msvc then
	module:add_files( "source/win/*.cpp" );
elseif use_mingw then
	module:add_files( "source/linux/*.cpp" );
else
	throw("Platform not supported.");
end