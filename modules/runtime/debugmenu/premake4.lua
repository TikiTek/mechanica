-- library/modules/runtime/debugmenu

local module = Module:new( "debugmenu" );

module:add_files( "include/**/*.hpp" );
module:add_files( "source/*.*" );
module:add_include_dir( "include" );

module:add_dependency( "graphics" );
