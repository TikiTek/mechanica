-- locale/game

local game = Module:new( "game" );

game:add_files( "source/*.cpp" );
game:add_files( "include/**/*.hpp" );
game:add_include_dir( "include" );

game:add_dependency( "base" );
game:add_dependency( "framework" );
game:add_dependency( "gameflow" );
game:add_dependency( "gamestates" );

local game_project = Project:new(
	"game",
	{ "x32", "x64" },
	{ "Debug", "Release", "Master" },
	game,
	ProjectTypes.windowApplication
);