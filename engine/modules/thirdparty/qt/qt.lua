-- library/thirdparty/qt

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

module.import_func = function(project)
	--local output_path = path.getabsolute( _OPTIONS[ "qt_dir" ] )
	--
	--module:add_include_dir( output_path );
	--	

	--
	--for i,file_name in pairs( module.ui_files ) do
	--	
	--	module:add_files( output_file );
	--end
	--
	--for i,file_name in pairs( module.moc_files ) do
	--	local output_file = path.join( output_path, "moc_" .. path.getbasename( file_name ) .. ".cpp" )
	--	local input_file = path.getabsolute( file_name )
	--	local command_line = moc_exe .. " \"" .. input_file .. "\" -o \"" .. output_file .. "\" " .. moc_defines .. " " .. moc_includes
    --
	--	print( "Moc: " .. file_name );
	--	prebuildcommands{ command_line }
	--	os.execute( command_line );
	--	
	--	module:add_files( output_file );
	--end
end
