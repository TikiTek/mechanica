-- local/modules/gameplay

local module = Module:new( "gameplay" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );

module:add_dependency( "entitysystem" );
module:add_dependency( "components" );
module:add_dependency( "gamecomponents" );
