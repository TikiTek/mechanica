-- library/thirdparty/trex

local module = Module:new( "trex" );

module.module_type = ModuleTypes.UnityCModule;

module:add_files( "source/*.*" );
module:add_files( "include/*.h" );
module:add_include_dir( "include" );
