-- library/modules/editor/packageeditor

local module = Module:new( "packageeditor" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "editor/editorinterface" );
module:add_dependency( "thirdparty/qt" );

module.import_func = function(project)
	module:add_moc_file( "include/tiki/packageeditor/packageeditorwidget.hpp" )
end
