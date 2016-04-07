-- library/thirdparty/trex

local module = Module:new( "trex" );

module.module_type = ModuleTypes.UnityCModule;

module:add_files( "trex.lua" );
module:set_base_path( "blobs/trex" );

module:add_files( "*.h" );
module:add_files( "trex.c" );
module:add_include_dir( "." );
