-- library/thirdparty/curl

local module = Module:new( "curl" );

module.module_type = ModuleTypes.FilesModule;

module:add_files( "include/**.h" );
module:add_include_dir( "include" );
module:add_include_dir( "include/curl" );
module:add_files( "source/*.c" );
module:add_files( "curl.lua" );
module:set_base_path( "blobs/curl" );

module:add_files( "include/**.h" );
module:add_include_dir( "include" );
module:add_include_dir( "lib" );

module:add_files( "src/*.c" );
module:add_files( "src/*.h" );
module:add_files( "src/*.rc" );

module:add_files( "src/tool_main.c", { exclude = true } );

module:add_files( "lib/*.c" );
module:add_files( "lib/*.h" );

module:add_files( "lib/vauth/*.c" );
module:add_files( "lib/vauth/*.h" );
module:add_files( "lib/vtls/*.c" );
module:add_files( "lib/vtls/*.h" );

module:add_files( "lib/strtoofft.c" );
module:add_files( "lib/rawstr.c" );
module:add_files( "lib/nonblock.c" );
module:add_files( "lib/warnless.c" );
module:add_files( "lib/config-win32.h" );
module:add_files( "lib/curl_setup.h" );
module:add_files( "lib/strtoofft.h" );
module:add_files( "lib/rawstr.h" );
module:add_files( "lib/nonblock.h" );
module:add_files( "lib/warnless.h" );

module:set_define( "CURL_STATICLIB" );

module:add_library_file( "ws2_32" );
module:add_library_file( "wldap32" );
