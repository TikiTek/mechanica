local module = Module:new();

module:add_files( "*.lua" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );
module:add_files( "source/*.cpp" );

module:add_dependency( "core/base" );
module:add_dependency( "tool/tool_application" );
