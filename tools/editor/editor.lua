-- library/tools/editor

local module = Module:new( "editor" );

module:add_files( "source/*.*" );
module:add_files( "*.lua" );

module:add_dependency( "config" );
module:add_dependency( "core/resource" );
module:add_dependency( "core/genericdata" );

module:add_dependency( "tool/toolbase" );
module:add_dependency( "tool/toolapplication" );

module:add_dependency( "editor/editor_interface" );
module:add_dependency( "editor/package_editor" );
module:add_dependency( "editor/generic_data_editor" );
module:add_dependency( "editor/converter_editor" );

module:add_resources( "resource/*.png" );
