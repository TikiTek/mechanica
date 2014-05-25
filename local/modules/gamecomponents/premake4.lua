-- local/modules/gamecomponents

local module = Module:new( "gamecomponents" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );

module:add_dependency( "componentbase" );
module:add_dependency( "gamecomponentdata" );
module:add_dependency( "physics" );
