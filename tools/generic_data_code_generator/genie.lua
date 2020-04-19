-- library/tools/generic_data_code_generator

include "../../.."

local module = Module:new( "generic_data_code_generator" );

module:add_files( "source/*.*" );
module:add_files( "*.lua" );

module:add_dependency( "config" );
module:add_dependency( "tool/tool_base" );
module:add_dependency( "tool/tool_generic_data" );

local project = Project:new(
	"generic_data_code_generator",
	"18544de3-a9d2-41b9-82dd-3d3077e320ff",
	{ "x32", "x64" },
	{ "Debug", "Release" },
	module,
	ProjectTypes.consoleApplication
);

project:add_install( "*.exe", "library/buildtools/generic_data_code_generator", "Release" );

local solution = Solution:new( "generic_data_code_generator" );
solution:add_project( project );

solution:finalize();
