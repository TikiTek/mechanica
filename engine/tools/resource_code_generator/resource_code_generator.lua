local module = Module:new( "resource_code_generator" );

module:add_files( "source/*.*" );
module:add_files( "*.lua" );

module:add_dependency( "config" );
module:add_dependency( "tool/toolbase" );
