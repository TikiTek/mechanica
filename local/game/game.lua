-- local/game

local game = Module:new( "game" );

game:add_files( "source/*.cpp" );
game:add_files( "include/**/*.hpp" );
game:add_include_dir( "include" );

game:add_dependency( "config" );
game:add_dependency( "base" );
game:add_dependency( "framework" );
game:add_dependency( "gameflowsystem" );
game:add_dependency( "gamestates" );
game:add_dependency( "genericdata" );

game:set_define( "TIKI_BUILD_TOOLS", "TIKI_OFF" );

game:add_library_file( "converterlibrary" );
game:add_library_file( "webinterfacelibrary" );

local game_project = Project:new(
	"game",
	{ "x32", "x64" },
	{ "Debug", "Release", "Master" },
	game,
	ProjectTypes.windowApplication
);