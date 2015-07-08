-- library/modules/runtime/graphicsresources

local module = Module:new( "graphicsresources" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );
