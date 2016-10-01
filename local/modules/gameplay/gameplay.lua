-- local/modules/gameplay

local module = Module:new( "gameplay" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "gameplay.lua" );
module:add_include_dir( "include" );

module:add_dependency( "entitysystem" );
module:add_dependency( "components" );
module:add_dependency( "gamecomponents" );
module:add_dependency( "physics3dcomponents" );
module:add_dependency( "debugrenderer" );
module:add_dependency( "voxelworld" );
module:add_dependency( "voxelmesh" );
