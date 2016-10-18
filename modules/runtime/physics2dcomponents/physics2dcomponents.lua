-- library/modules/runtime/physics2dcomponents

local module = Module:new( "physics2dcomponents" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "components2d" );
module:add_dependency( "entitysystem" );
module:add_dependency( "physics2d" );
