-- library/thirdparty/fcpp

local module = Module:new( "fcpp" );

module.module_type = ModuleTypes.FilesModule;

module:add_files( "source/cpp*.c" );
module:add_files( "include/*.h" );
module:add_include_dir( "include" );
