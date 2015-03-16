-- library/thirdparty/mongoose

local module = Module:new( "mongoose" );

module.module_type = ModuleTypes.UnityCModule;

module:add_files( "inc/*.h" );
module:add_include_dir( "inc" );

module:add_files( "src/*.c" );

module:set_define( "MONGOOSE_ENABLE_THREADS" );

--module:add_library_file( "ws2_32" );
--module:add_library_file( "wldap32" );
