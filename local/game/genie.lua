-- local/game/project

include "../.."

local module = Module:new( "game" );

module:add_files( "*.lua" );

module:add_files( "source/*.cpp" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );

module:add_dependency( "config" );
module:add_dependency( "core/base" );
module:add_dependency( "core/generic_data" );
module:add_dependency( "runtime/game_application" );
module:add_dependency( "runtime/game_flow" );
module:add_dependency( "runtime/physics_2d" );

module:add_dependency( "runtime/mechanica_game_states" );

local project = Project:new(
	"game",
	"6fc90403-3d7c-4a18-bbf7-836585a92f5c",
	{ "x32", "x64" },
	{ "Debug", "Release", "Master" },
	module,
	ProjectTypes.windowApplication
);

local solution = Solution:new( "game" );
solution:add_project( project );
solution:finalize();
