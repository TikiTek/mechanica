-- library/thirdparty/tinyxml2

local module = Module:new( "tinyxml2" );

module.module_type = ModuleTypes.FilesModule;

module:add_files( "*.lua" );
module:set_base_path( "blobs/tinyxml2" );

module:add_files( "tinyxml2.h" );
module:add_files( "tinyxml2.cpp" );

module:add_include_dir( "." );
