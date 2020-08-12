-- library/modules/runtime/debugrenderer

local module = Module:new( "debugrenderer" );

module:add_files( "include/**/*.hpp" );
module:add_files( "source/*.*" );
module:add_files( "debugrenderer.lua" );
module:add_include_dir( "include" );

module:add_dependency( "graphics" );
