-- library/modules/editor/genericdataeditor

local module = Module:new( "genericdataeditor" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "tool/toolgenericdata" );
module:add_dependency( "editor/editorinterface" );
module:add_dependency( "thirdparty/qt" );

module.import_func = function(project)
	module:add_moc_files( "include/tiki/genericdataeditor/*editor.hpp" )
	module:add_moc_files( "source/*widget.hpp" )
	module:add_moc_files( "source/*editor.hpp" )

	module:add_qrc_files( "source/*.qrc" )
end
