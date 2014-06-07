-- library/modules/core/base

local module = Module:new( "base" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );

if is_windows then
	module:add_files( "source/win/*.cpp" );
end