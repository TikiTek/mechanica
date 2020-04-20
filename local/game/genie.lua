-- local/game

include( "../.." );

local project = Project:new(
	"game",
	"6fc90403-3d7c-4a18-bbf7-836585a92f5c",
	{ "x32", "x64" },
	{ "Debug", "Release", "Master" },
	"local/game",
	ProjectTypes.windowApplication
);

local solution = Solution:new();
solution:add_project( project );
solution:finalize();
