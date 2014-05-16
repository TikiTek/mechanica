-- library/modules/runtime/physics

local module = Module:new( "physics" );

module:set_define( "BT_NO_PROFILE" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );

module:add_dependency( "bullet" );
