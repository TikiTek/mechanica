local module = Module:new();

module:add_files( "*.lua" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );

module:add_dependency( "tool/tool_base" );
module:add_dependency( "tool/tool_xml" );
module:add_dependency( "tool/tool_project" );
