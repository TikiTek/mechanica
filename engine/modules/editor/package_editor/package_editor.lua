-- library/modules/editor/package_editor

local module = Module:new( "package_editor" );

module:add_files( "*.lua" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );
module:add_files( "source/*.*" );

module:add_dependency( "editor/editor_interface" );
module:add_dependency( "thirdparty/qt" );
module:add_dependency( "thirdparty/tinyxml2" );

module.import_func = function(project)
	module:add_moc_files( "include/tiki/package_editor/*editor.hpp" )
	module:add_moc_files( "include/tiki/package_editor/*widget.hpp" )
	
	module:add_qrc_files( "source/*.qrc" )
end
