-- local/game/project

include "../../../library/buildtools/genie_scripts"

add_module_include_path( "../.." )

local project = Project:new(
	"game",
	"6fc90403-3d7c-4a18-bbf7-836585a92f5c",
	{ "x32", "x64" },
	{ "Debug", "Release", "Master" },
	find_module( "game" ),
	ProjectTypes.windowApplication
);

local solution = Solution:new( "game" );
solution:add_project( project );
--solution:add_project( find_project( "converterlibrary" ) );

--if use_msvc then
--	solution:add_project( find_project( "webinterfacelibrary" ) );
--end

solution:finalize();
