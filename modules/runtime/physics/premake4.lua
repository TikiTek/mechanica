-- library/modules/runtime/physics

local module = Module:new( "physics" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );

module:add_dependency( "bullet" );
