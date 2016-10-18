-- local/modules/gamestates

local module = Module:new( "gamestates" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "framework" );
module:add_dependency( "graphics" );
module:add_dependency( "resource" );
module:add_dependency( "renderer" );
module:add_dependency( "gameflowsystem" );
module:add_dependency( "gameplay" );
module:add_dependency( "particle" );
