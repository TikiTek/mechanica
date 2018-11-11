-- library/modules/runtime/physics3d

local module = Module:new( "physics3d" );

module:add_files( "physics3d.lua" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );

module:set_define( "BT_NO_PROFILE" );
module:add_dependency( "bullet" );
