-- local/modules/mechanica_components

local module = Module:new( "mechanica_components" );

module:add_files( "*.lua" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );

module:add_dependency( "runtime/component_base" );
module:add_dependency( "runtime/physics_2d_components" );
