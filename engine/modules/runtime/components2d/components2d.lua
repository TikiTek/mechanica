-- library/modules/runtime/components2d

local module = Module:new( "components2d" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "componentbase" );
module:add_dependency( "entitysystem" );
