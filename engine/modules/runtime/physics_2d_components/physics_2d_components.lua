local module = Module:new();

module:add_files( "*.lua" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );

module:add_dependency( "runtime/components_2d" );
module:add_dependency( "runtime/entity_system" );
module:add_dependency( "runtime/physics_2d" );
