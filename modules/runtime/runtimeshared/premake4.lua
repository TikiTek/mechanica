-- library/modules/runtime/runtimeshared

local module = Module:new( "runtimeshared" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );

module:add_dependency( "graphicsresources" );
module:add_dependency( "resource" );
