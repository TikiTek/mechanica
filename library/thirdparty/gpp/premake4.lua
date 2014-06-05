-- library/thirdparty/gpp

local module = Module:new( "gpp" );

module.module_type = ModuleTypes.UnityCModule;

module:add_files( "src/*.c" );
module:add_files( "include/*.h" );
module:add_include_dir( "include" );
