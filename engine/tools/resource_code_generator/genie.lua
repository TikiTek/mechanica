-- engine/tools/resource_code_generator

include( "../../.." );
add_module_include_path( "../.." );

local project = Project:new(
	"resource_code_generator",
	"c2d4d778-6f7a-4f18-b601-c4619ce5a3c1",
	{ "x32", "x64" },
	{ "Debug", "Release" },
	"tools/resource_code_generator",
	ProjectTypes.consoleApplication
);

project:add_install( "*.exe", "library/buildtools/resource_code_generator", "Release" );

local solution = Solution:new();
solution:add_project( project );
solution:finalize();
