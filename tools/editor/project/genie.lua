-- library/tools/editor/project

include( "../../../buildtools/genie_scripts" );
add_extension( "qt" );

dofile( "../editor.lua" );

local project = Project:new(
	"editor",
	"a51d9621-4869-4572-827d-b3e201305cbc",
	{ "x32", "x64" },
	{ "Debug", "Release"},
	find_module( "editor" ),
	ProjectTypes.windowApplication
);

local solution = Solution:new( "editor" );
solution:add_project( project );
solution:finalize();
