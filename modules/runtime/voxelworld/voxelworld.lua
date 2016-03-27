-- library/modules/runtime/voxelworld

local module = Module:new( "voxelworld" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "math" );
