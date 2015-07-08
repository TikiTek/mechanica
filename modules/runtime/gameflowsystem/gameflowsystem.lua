-- library/modules/runtime/gameflowsystem

local module = Module:new( "gameflowsystem" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );

module:add_dependency( "base" );
