-- library/modules/editor/generic_data_editor

local module = Module:new( "generic_data_editor" );

module:add_files( "*.lua" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );
module:add_files( "source/*.*" );

module:add_dependency( "tool/toolgenericdata" );
module:add_dependency( "editor/editor_interface" );

module:add_resources( "resource/*.png" );
