-- library/tools/editor

include( "../../.." );
add_extension( "resources" );

local module = Module:new( "editor" );

module:add_files( "source/*.*" );
module:add_files( "*.lua" );

module:add_dependency( "config" );
module:add_dependency( "core/resource" );
module:add_dependency( "core/generic_data" );

module:add_dependency( "tool/tool_base" );
module:add_dependency( "tool/tool_application" );

module:add_dependency( "editor/editor_interface" );
module:add_dependency( "editor/package_editor" );
module:add_dependency( "editor/generic_data_editor" );
module:add_dependency( "editor/converter_editor" );
module:add_dependency( "editor/entity_template_editor" );

module:add_resources( "resource/*.png" );

local project = Project:new(
	"editor",
	"a51d9621-4869-4572-827d-b3e201305cbc",
	{ "x32", "x64" },
	{ "Debug", "Release"},
	module,
	ProjectTypes.windowApplication
);

local solution = Solution:new( "editor" );
solution:add_project( project );
solution:finalize();
