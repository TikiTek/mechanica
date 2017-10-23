-- library/tools/genericdatacodegenerator/project

include "../../../buildtools/genie_scripts"

dofile( "../genericdatacodegenerator.lua" );

local project = Project:new(
	"genericdatacodegenerator",
	"18544de3-a9d2-41b9-82dd-3d3077e320ff",
	{ "x32", "x64" },
	{ "Debug", "Release" },
	find_module( 'genericdatacodegenerator' ),
	ProjectTypes.consoleApplication
);

local solution = Solution:new( "genericdatacodegenerator" );
solution:add_project( project );
solution:finalize();
