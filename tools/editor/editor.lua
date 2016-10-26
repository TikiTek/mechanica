-- library/tools/editor

local module = Module:new( "editor" );

module:add_files( "source/*.*" );
module:add_files( "*.lua" );

module:add_dependency( "config" );
module:add_dependency( "core/resource" );
module:add_dependency( "core/genericdata" );
module:add_dependency( "runtime/graphics" );
module:add_dependency( "runtime/graphicsresources" );
module:add_dependency( "tool/toolbase" );
module:add_dependency( "tool/qtapplication" );

--module:add_dependency( "toollibraries" );

--module:add_ui_file( "source/mainwindow.ui" )
module:add_moc_file( "source/editorwindow.hpp" )

--module:add_library_file( "converterlibrary" );
