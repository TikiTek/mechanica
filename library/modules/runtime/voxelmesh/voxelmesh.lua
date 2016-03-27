-- library/modules/runtime/voxelmesh

local module = Module:new( "voxelmesh" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "voxelworld" );
