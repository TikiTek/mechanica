-- library/thirdparty/box2d

local module = Module:new( "box2d" );

module.module_type = ModuleTypes.FilesModule;

module:add_files( "box2d.lua" );
module:set_base_path( "blobs/box2d" );

module:add_files( "Box2D/Box2/**/*.h" );
module:add_files( "Box2D/Box2/**/*.cpp" );

module:add_include_dir( "Box2D" );
