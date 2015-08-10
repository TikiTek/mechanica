-- library/modules/core/math

local module = Module:new( "math" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );

module:add_dependency( "genericdata" );
