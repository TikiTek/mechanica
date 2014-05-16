-- library/thirdparty/bullet

local module = Module:new( "bullet" );

module.module_type = ModuleTypes.FilesModule;

module:add_include_dir( "src" );

module:add_files( "src/**/*.*" );
