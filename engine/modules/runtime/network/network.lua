local module = Module:new( "network" );

module:add_files( "include/**/*.*" );
module:add_files( "source/*.*" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "core/base" );

module:add_library_file( "ws2_32" );
