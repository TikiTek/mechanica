-- library/tools/resource_code_generator/project

include "../../.."

local module = Module:new( "resource_code_generator" );

module:add_files( "source/*.*" );
module:add_files( "*.lua" );

module:add_dependency( "config" );
module:add_dependency( "tool/tool_base" );

local project = Project:new(
	"resource_code_generator",
	"c2d4d778-6f7a-4f18-b601-c4619ce5a3c1",
	{ "x32", "x64" },
	{ "Debug", "Release" },
	module,
	ProjectTypes.consoleApplication
);

local solution = Solution:new( "resource_code_generator" );
solution:add_project( project );
solution:finalize();
