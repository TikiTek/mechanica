local module = Module:new();

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "tool/tool_generic_data" );
module:add_dependency( "editor/editor_interface" );
module:add_dependency( "editor/generic_data_editor" );