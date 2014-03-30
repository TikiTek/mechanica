-- library/modules/runtime/debuggui

local module = Module:new( "debuggui" );

module:add_files( "include/**/*.hpp" );
module:add_files( "source/*.*" );
module:add_include_dir( "include" );

module:add_dependency( "graphics" );
