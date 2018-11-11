local module = Module:new();

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "core/base" );
module:add_dependency( "core/threading" );
module:add_dependency( "core/resource" );
module:add_dependency( "runtime/task_system" );
module:add_dependency( "tool/tool_base" );
module:add_dependency( "tool/tool_sql" );

