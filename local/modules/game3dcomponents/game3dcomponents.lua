-- local/modules/game3dcomponents

local module = Module:new( "game3dcomponents" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "gamecomponents.lua" );
module:add_include_dir( "include" );

module:add_dependency( "componentbase" );
module:add_dependency( "physics3d" );
