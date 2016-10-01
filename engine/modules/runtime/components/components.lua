-- library/modules/runtime/components

local module = Module:new( "components" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "components.lua" );
module:add_include_dir( "include" );

module:add_dependency( "componentbase" );
module:add_dependency( "entitysystem" );
