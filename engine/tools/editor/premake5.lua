-- engine/tools/editor

include( "../../.." );
tiki.add_extension( "resources" );

local project = Project:new( "editor", ProjectTypes.WindowApplication );

project:add_files( "source/*.*" );
project:add_files( "*.lua" );

project:add_files( "icon/*.h" );
project:add_files( "icon/*.rc" );

project:add_dependency( "config" );
project:add_dependency( "core/resource" );
project:add_dependency( "core/generic_data" );

project:add_dependency( "tool/tool_base" );
project:add_dependency( "tool/tool_application" );

project:add_dependency( "editor/editor_interface" );
project:add_dependency( "editor/package_editor" );
project:add_dependency( "editor/generic_data_editor" );
project:add_dependency( "editor/converter_editor" );
project:add_dependency( "editor/entity_template_editor" );

project:add_resources( "resource/*.png" );

finalize_default_solution( project )
