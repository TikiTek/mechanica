-- library/tools/command_line_converter/project

include "../../../buildtools/genie_scripts"

dofile( "../command_line_converter.lua" );

local project = Project:new(
	"command_line_converter",
	"575bd0eb-4477-443f-8b4f-5c8a1f72bf86",
	{ "x32", "x64" },
	{ "Debug", "Release" },
	module,
	ProjectTypes.consoleApplication
);

local solution = Solution:new( "command_line_converter" );
solution:add_project( project );
solution:finalize();
