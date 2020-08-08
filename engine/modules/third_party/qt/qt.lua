local module = Module:new( "qt" );

module.module_type = ModuleTypes.FilesModule;

module:add_files( "*.lua" );
module:set_base_path( qt_dir );

module:add_files( "include/QtCore/*" );
module:add_files( "include/QtGui/*" );
module:add_files( "include/QtWidgets/*" );
module:add_include_dir( "include" );
module:add_include_dir( "include/QtCore" );
module:add_include_dir( "include/QtGui" );
module:add_include_dir( "include/QtWidgets" );

module:add_library_dir( "lib" );

module:add_library_file( "Qt5Cored", "Debug" );
module:add_library_file( "Qt5Guid", "Debug" );
module:add_library_file( "Qt5Widgetsd", "Debug" );
module:add_library_file( "Qt5Core", "Release" );
module:add_library_file( "Qt5Gui", "Release" );
module:add_library_file( "Qt5Widgets", "Release" );

module:add_binary_dir( "bin" );

module:add_binary_file( "Qt5Cored.dll", "Debug" );
module:add_binary_file( "Qt5Widgetsd.dll", "Debug" );
module:add_binary_file( "Qt5Guid.dll", "Debug" );
module:add_binary_file( "Qt5Core.dll", "Release" );
module:add_binary_file( "Qt5Widgets.dll", "Release" );
module:add_binary_file( "Qt5Gui.dll", "Release" );
