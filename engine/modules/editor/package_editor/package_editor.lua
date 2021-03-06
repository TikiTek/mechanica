local module = Module:new();

module:add_files( "*.lua" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );
module:add_files( "source/*.*" );

module:add_dependency( "editor/editor_interface" );
module:add_dependency( "tool/tool_xml" );

module:add_resources( "resource/*.png" );
