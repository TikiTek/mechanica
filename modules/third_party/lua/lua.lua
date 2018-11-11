-- library/thirdparty/lua

local module = Module:new( "lua" );

module.module_type = ModuleTypes.FilesModule;

module:add_files( "lua.lua" );
module:set_base_path( "blobs/lua" );

module:add_files( "src/*.h" );
module:add_files( "src/*.c" );
module:add_files( "src/lua.c", {exclude = true} );
module:add_files( "src/luac.c", {exclude = true} );

module:add_include_dir( "src" );
