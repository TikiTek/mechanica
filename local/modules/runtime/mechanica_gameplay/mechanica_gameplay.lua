-- local/modules/mechanica_gameplay

local module = Module:new( "mechanica_gameplay" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "runtime/entity_system" );
module:add_dependency( "runtime/components" );
module:add_dependency( "runtime/physics_2d_components" );
module:add_dependency( "runtime/debug_renderer" );

module:add_dependency( "runtime/mechanica_components" );
