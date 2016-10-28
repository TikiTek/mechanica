-- library/tools/editor

local module = Module:new( "editor" );

module:add_files( "source/*.*" );
module:add_files( "*.lua" );

module:add_dependency( "config" );
module:add_dependency( "core/resource" );
module:add_dependency( "core/genericdata" );
module:add_dependency( "tool/toolbase" );
module:add_dependency( "tool/qtapplication" );

--module:add_dependency( "toollibraries" );

--module:add_ui_file( "source/mainwindow.ui" )


--module:add_library_file( "converterlibrary" );

module.import_func = function(project)
	module:add_moc_file( "source/editorwindow.hpp" )
end
