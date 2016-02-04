-- library/thirdparty/glew

local module = Module:new( "glew" );

module.module_type = ModuleTypes.UnityCModule;

module:add_files( "src/*.c" );
module:add_files( "include/*.h" );
module:add_files( "glew.lua" );
module:add_include_dir( "include" );

module:set_define( "GLEW_STATIC" );
