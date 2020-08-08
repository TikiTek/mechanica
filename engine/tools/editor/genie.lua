-- library/tools/editor

include( "../../.." );
add_extension( "resources" );
add_module_include_path( "../.." );

local project = Project:new(
	"editor",
	"a51d9621-4869-4572-827d-b3e201305cbc",
	{ "x32", "x64" },
	{ "Debug", "Release"},
	"tools/editor",
	ProjectTypes.windowApplication
);

local solution = Solution:new();
solution:add_project( project );
solution:finalize();
