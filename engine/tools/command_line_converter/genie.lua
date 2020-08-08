-- engine/tools/command_line_converter

include( "../../.." );
add_module_include_path( "../.." );

local project = Project:new(
	"command_line_converter",
	"575bd0eb-4477-443f-8b4f-5c8a1f72bf86",
	{ "x32", "x64" },
	{ "Debug", "Release" },
	"tools/command_line_converter",
	ProjectTypes.consoleApplication
);

local solution = Solution:new();
solution:add_project( project );
solution:finalize();
