local module = Module:new();

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