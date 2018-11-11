-- library/modules/editor/genericdataeditor

local module = Module:new( "genericdataeditor" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "tool/toolgenericdata" );
module:add_dependency( "editor/editor_interface" );
module:add_dependency( "editor/generic_data_editor" );