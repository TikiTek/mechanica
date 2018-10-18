-- library/tools/command_line_converter

local module = Module:new( "command_line_converter" );

module:add_files( "source/*.*" );
module:add_files( "command_line_converter.lua" );

module:add_dependency( "config" );
module:add_dependency( "toollibraries" );
module:add_dependency( "renderer2dshader" );

module:set_define( "TIKI_BUILD_TOOLS", "TIKI_OFF" );

module:add_library_file( "converterlibrary" );
