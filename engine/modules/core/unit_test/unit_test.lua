local module = Module:new();

module:add_files( "include/**/*.*" );
module:add_files( "source/*.*" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "base" );
module:add_dependency( "toolbase" );
