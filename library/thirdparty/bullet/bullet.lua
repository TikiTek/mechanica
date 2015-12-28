-- library/thirdparty/bullet

local module = Module:new( "bullet" );

module.module_type = ModuleTypes.FilesModule;

module:add_files( "src/**" );
module:add_files( "src/**/**" );
module:add_files( "src/**/**/**" );
module:add_files( "bullet.lua" );

module:add_include_dir( "src" );
