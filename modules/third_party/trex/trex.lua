local module = Module:new();

module.module_type = ModuleTypes.UnityCModule;

module:add_files( "*.lua" );
module:set_base_path( "third_party/trex" );

module:add_files( "*.h" );
module:add_files( "trex.c" );
module:add_include_dir( "." );
