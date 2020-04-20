local module = Module:new();

module:add_files( "source/*.*" );
module:add_files( "*.lua" );

module:add_dependency( "config" );
module:add_dependency( "tool/tool_base" );
module:add_dependency( "tool/tool_generic_data" );
