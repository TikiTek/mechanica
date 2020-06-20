local module = Module:new();

module:add_files( "*.lua" );
module:set_base_path( "third_party/stb" );

module:add_files( "*.c" );
module:add_files( "*.h" );

module:add_include_dir( "." );

if is_linux then
	module:add_library_file( "dl" );
end

module:set_define( "SQLITE_THREADSAFE", "0" );
