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
	module:add_moc_file( "include/tiki/genericdataeditor/genericdataeditor.hpp" )
	module:add_moc_file( "source/genericdataboolvaluewidget.hpp" )
	module:add_moc_file( "source/genericdataeditorwidget.hpp" )
	module:add_moc_file( "source/genericdatanumbervaluewidget.hpp" )
	module:add_moc_file( "source/genericdatareferencevaluewidget.hpp" )
	module:add_moc_file( "source/genericdatastringvaluewidget.hpp" )
	module:add_moc_file( "source/genericdatavaluebasewidget.hpp" )
	module:add_moc_file( "source/genericdatavaluetageditor.hpp" )

	module:add_qrc_file( "source/genericdataeditor.qrc" )
end
