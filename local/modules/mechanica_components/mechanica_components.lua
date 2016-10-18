-- local/modules/mechanica_components

local module = Module:new( "mechanica_components" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "componentbase" );
module:add_dependency( "physics2dcomponents" );
