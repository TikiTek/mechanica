-- library/modules/editor/editor_interface

local module = Module:new( "editor_interface" );

module:add_files( "*.lua" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );
module:add_files( "source/*.cpp" );

module:add_dependency( "core/base" );
module:add_dependency( "tool/toolapplication" );
