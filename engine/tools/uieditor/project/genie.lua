-- library/tools/uieditor/project

include "../../../buildtools/genie_scripts"

dofile( "../uieditor.lua" );

local project = Project:new(
	"uieditor",
	"ce188937-8610-427a-9397-fc75e2d584c3",
	{ "x32", "x64" },
	{ "Debug", "Release" },
	find_module( 'uieditor' ),
	ProjectTypes.windowApplication
);

local solution = Solution:new( "uieditor" );
solution:add_project( project );
--solution:add_project( find_project( "converterlibrary" ) );

solution:finalize();
