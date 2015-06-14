-- library/modules/runtime/particlebase

local module = Module:new( "particlebase" );

module:add_files( "include/**/*.*" );
module:add_include_dir( "include" );

module:add_dependency( "base" );
module:add_dependency( "math" );
