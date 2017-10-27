-- library/tools/editor

local module = Module:new( "editor" );

module:add_files( "source/*.*" );
module:add_files( "*.lua" );

module:add_dependency( "config" );
module:add_dependency( "core/resource" );
module:add_dependency( "core/genericdata" );

module:add_dependency( "tool/toolbase" );

module:add_dependency( "qt/qtapplication" );

module:add_dependency( "editor/editor_interface" );
module:add_dependency( "editor/package_editor" );
module:add_dependency( "editor/generic_data_editor" );
module:add_dependency( "editor/converter_editor" );

module.import_func = function(project)
	--module:add_ui_files( "source/mainwindow.ui" )

	module:add_moc_files( "source/editor.hpp" )
	module:add_moc_files( "source/editor_window.hpp" )
end
