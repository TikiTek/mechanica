-- library/modules/runtime/framework

local module = Module:new( "framework" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );

module:set_define( "TIKI_WINDOWEVENTBUFFER_SIZE", "32" );
module:set_define( "TIKI_WEB_INTERFACE", "TIKI_ON", "Debug" );

module:add_dependency( "graphics" );
module:add_dependency( "resource" );
module:add_dependency( "input" );
module:add_dependency( "debuggui" );
module:add_dependency( "debugguiwindows" );
module:add_dependency( "toollibraries" );
