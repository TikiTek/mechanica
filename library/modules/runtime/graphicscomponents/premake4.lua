-- library/modules/runtime/graphicscomponents

local module = Module:new( "graphicscomponents" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );

module:add_dependency( "componentbase" );
module:add_dependency( "componentdata" );
