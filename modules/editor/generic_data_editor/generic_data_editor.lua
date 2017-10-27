-- library/modules/editor/generic_data_editor

local module = Module:new( "generic_data_editor" );

module:add_files( "*.lua" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );
module:add_files( "source/*.*" );

module:add_dependency( "tool/toolgenericdata" );
module:add_dependency( "editor/editor_interface" );
module:add_dependency( "thirdparty/qt" );

module.import_func = function(project)
	module:add_moc_files( "include/tiki/generic_data_editor/*editor.hpp" )
	module:add_moc_files( "source/*widget.hpp" )
	module:add_moc_files( "source/*editor.hpp" )

	module:add_qrc_files( "source/*.qrc" )
end
