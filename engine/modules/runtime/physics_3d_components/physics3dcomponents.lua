local module = Module:new( "physics3dcomponents" );

module:add_files( "physics3dcomponents.lua" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );

module:add_dependency( "runtime/components3d" );
module:add_dependency( "runtime/entitysystem" );
module:add_dependency( "runtime/physics3d" );
