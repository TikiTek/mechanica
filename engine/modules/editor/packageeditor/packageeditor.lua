-- library/modules/editor/packageeditor

local module = Module:new( "packageeditor" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "editor/editorinterface" );
module:add_dependency( "thirdparty/qt" );
module:add_dependency( "thirdparty/tinyxml2" );

module.import_func = function(project)
	module:add_moc_files( "include/tiki/packageeditor/*editor.hpp" )
	module:add_moc_files( "include/tiki/packageeditor/*widget.hpp" )
	
	module:add_qrc_files( "source/*.qrc" )
end
