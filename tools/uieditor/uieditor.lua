-- library/tools/uieditor

local module = Module:new( "uieditor" );

module:add_files( "include/**/*.*" );
module:add_include_dir( "include" );
module:add_files( "uieditor.lua" );
module:add_files( "source/*.*" );

module:add_dependency( "config" );
module:add_dependency( "tool/toolbase" );
module:add_dependency( "tool/toolapplication" );
--module:add_dependency( "tool/toolui" );

module:set_define( "TIKI_BUILD_TOOLS", "TIKI_ON" );
