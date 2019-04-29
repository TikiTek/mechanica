-- library/tools/generic_data_code_generator/project

include "../../../buildtools/genie_scripts"

dofile( "../generic_data_code_generator.lua" );

local project = Project:new(
	"generic_data_code_generator",
	"18544de3-a9d2-41b9-82dd-3d3077e320ff",
	{ "x32", "x64" },
	{ "Debug", "Release" },
	find_module( 'generic_data_code_generator' ),
	ProjectTypes.consoleApplication
);

project:add_install( "*.exe", "library/buildtools/generic_data_code_generator", "Release" );

local solution = Solution:new( "generic_data_code_generator" );
solution:add_project( project );

solution:finalize();
