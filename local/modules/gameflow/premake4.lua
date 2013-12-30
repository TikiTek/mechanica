-- locale/modules/gameflow

local module = Module:new( "gameflow" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );

module:add_dependency( "base" );
