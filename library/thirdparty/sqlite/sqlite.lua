-- library/thirdparty/sqlite

local module = Module:new( "sqlite" );

module.module_type = ModuleTypes.UnityCModule;

module:add_files( "source/*.c" );
module:add_files( "include/**/*.h" );
module:add_files( "sqlite.lua" );
module:add_include_dir( "include" );

module:set_define( "SQLITE_THREADSAFE", "0" );
