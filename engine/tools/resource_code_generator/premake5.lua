-- engine/tools/resource_code_generator

include( "../../.." );

local project = Project:new(
	"resource_code_generator",
	{ "x32", "x64" },
	{ "Debug", "Release" },
	ProjectTypes.ConsoleApplication
);

project:add_files( "source/*.*" );
project:add_files( "*.lua" );

project:add_dependency( "config" );
project:add_dependency( "tool/tool_base" );

project:add_install( "*.exe", "../../buildtools/resource_code_generator", "Release" );

finalize_default_solution( project )
