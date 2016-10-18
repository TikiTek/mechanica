-- local/modules/gameplay

local module = Module:new( "gameplay" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "entitysystem" );
module:add_dependency( "components" );
module:add_dependency( "mechanica_components" );
module:add_dependency( "physics2dcomponents" );
module:add_dependency( "debugrenderer" );
