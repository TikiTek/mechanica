-- library/modules/runtime/entitysystem

local module = Module:new( "entitysystem" );

module:add_files( "include/**/*.hpp" );
module:add_files( "source/*.*" );
module:add_include_dir( "include" );

module:add_dependency( "base" );
