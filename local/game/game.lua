-- local/game

local module = Module:new( "game" );

module:add_files( "source/*.cpp" );
module:add_files( "include/**/*.hpp" );
module:add_files( "game.lua" );
module:add_include_dir( "include" );

module:add_dependency( "config" );
module:add_dependency( "base" );
module:add_dependency( "gameapplication" );
module:add_dependency( "gameflowsystem" );
module:add_dependency( "gamestates" );
module:add_dependency( "genericdata" );

module:set_define( "TIKI_BUILD_TOOLS", "TIKI_OFF" );

module:add_library_file( "converterlibrary" );
module:add_library_file( "webinterfacelibrary" );

local game_project = Project:new(
	"game",
	{ "x32", "x64" },
	{ "Debug", "Release", "Master" },
	module,
	ProjectTypes.windowApplication
);