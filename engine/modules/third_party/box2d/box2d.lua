local module = Module:new();

module.module_type = ModuleTypes.FilesModule;

module:add_files( "*.lua" );
module:set_base_path( "third_party/box2d" );

module:add_files( "include/box2d/*.h" );
module:add_files( "src/**/*.cpp" );

module:add_include_dir( "include" );
module:add_include_dir( "src" );
