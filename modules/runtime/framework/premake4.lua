-- library/modules/runtime/framework

local module = Module:new( "framework" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );

module:set_define( "TIKI_WEB_INTERFACE", "TIKI_ON", "Debug" );
module:set_define( "TIKI_WEB_INTERFACE", "TIKI_OFF", "Release" );
module:set_define( "TIKI_WEB_INTERFACE", "TIKI_OFF", "Master" );

module:add_dependency( "graphics" );
module:add_dependency( "resource" );
module:add_dependency( "input" );
module:add_dependency( "debuggui" );
module:add_dependency( "debugguiwindows" );
module:add_dependency( "webserverpages" );
module:add_dependency( "toollibraries" );
module:add_dependency( "runtimeshared" );