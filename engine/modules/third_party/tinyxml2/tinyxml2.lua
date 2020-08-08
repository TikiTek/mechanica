local module = Module:new();

module.module_type = ModuleTypes.FilesModule;

module:add_files( "*.lua" );
module:set_base_path( "third_party/tinyxml2" );

module:add_files( "tinyxml2.h" );
module:add_files( "tinyxml2.cpp" );

module:add_include_dir( "." );
