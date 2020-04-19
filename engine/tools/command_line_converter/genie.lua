-- library/tools/command_line_converter/project

include "../../.."

local module = Module:new( "command_line_converter" );

module:add_files( "source/*.*" );
module:add_files( "*.lua" );

module:add_dependency( "config" );
module:add_dependency( "tool/asset_converter_library" );

module:set_define( "TIKI_BUILD_TOOLS", "TIKI_OFF" );

module:add_library_file( "converterlibrary" );

local project = Project:new(
	"command_line_converter",
	"575bd0eb-4477-443f-8b4f-5c8a1f72bf86",
	{ "x32", "x64" },
	{ "Debug", "Release" },
	module,
	ProjectTypes.consoleApplication
);

local solution = Solution:new( "command_line_converter" );
solution:add_project( project );
solution:finalize();
