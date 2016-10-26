-- library/modules/runtime/qtapplication

local module = Module:new( "qtapplication" );

module:add_files( "include/**/*.*" );
module:add_include_dir( "include" );
module:add_files( "*.lua" );
module:add_files( "source/*.*" );

module:add_dependency( "core/base" );
module:add_dependency( "thirdparty/qt" );

module.import_func = function(project)
	module:add_moc_file( "include/tiki/qtapplication/qtmainwindow.hpp" );
	module:add_moc_file( "include/tiki/qtapplication/qtribbontab.hpp" );
	module:add_moc_file( "include/tiki/qtapplication/qtribbonbutton.hpp" );
	module:add_moc_file( "include/tiki/qtapplication/qtribbongroup.hpp" );
end