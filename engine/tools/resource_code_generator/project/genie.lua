-- library/tools/resource_code_generator/project

include "../../../buildtools/genie_scripts"

dofile( "../resource_code_generator.lua" );

local project = Project:new(
	"resource_code_generator",
	"c2d4d778-6f7a-4f18-b601-c4619ce5a3c1",
	{ "x32", "x64" },
	{ "Debug", "Release" },
	find_module( 'resource_code_generator' ),
	ProjectTypes.consoleApplication
);

local solution = Solution:new( "resource_code_generator" );
solution:add_project( project );
solution:finalize();
