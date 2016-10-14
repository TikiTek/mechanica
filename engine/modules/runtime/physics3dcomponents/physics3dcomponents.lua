-- library/modules/runtime/physics3dcomponents

local module = Module:new( "physics3dcomponents" );

module:add_files( "physics3dcomponents.lua" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );

module:add_dependency( "components3d" );
module:add_dependency( "entitysystem" );
module:add_dependency( "physics3d" );
