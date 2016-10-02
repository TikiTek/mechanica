-- library/modules/runtime/physics2d

local module = Module:new( "physics2d" );

module:add_files( "physics2d.lua" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );

module:add_dependency( "box2d" );
