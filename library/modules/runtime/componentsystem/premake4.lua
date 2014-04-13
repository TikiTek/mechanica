-- library/modules/runtime/componentsystem

local module = Module:new( "componentsystem" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );

module:add_dependency( "base" );
module:add_dependency( "componentbase" );
