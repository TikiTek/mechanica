local module = Module:new( "mongoose" );

module.module_type = ModuleTypes.UnityCModule;

module:add_files( "mongoose.lua" );
module:set_base_path( "blobs/mongoose" );

module:add_files( "*.h" );
module:add_files( "*.c" );
module:add_include_dir( "." );

module:set_define( "MONGOOSE_ENABLE_THREADS" );
