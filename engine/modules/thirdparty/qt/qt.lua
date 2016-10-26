-- library/thirdparty/qt

local module = Module:new( "qt" );
local qtDir = "C:\\Qt\\5.7\\msvc2015_64" --TODO: find real path e.g. os.getenv("QTDIR")

module.module_type = ModuleTypes.FilesModule;

module:add_files( "*.lua" );
module:set_base_path( qtDir );

module:add_files( "include/QtCore/*" );
module:add_files( "include/QtGui/*" );
module:add_files( "include/QtWidgets/*" );
module:add_include_dir( "include" );
module:add_include_dir( "include/QtCore" );
module:add_include_dir( "include/QtGui" );
module:add_include_dir( "include/QtWidgets" );

module:add_library_dir( "lib" );

module:add_library_file( "Qt5Core" );
module:add_library_file( "Qt5Gui" );
module:add_library_file( "Qt5Widgets" );

module:add_binary_dir( "bin" );

module:add_binary_file( "Qt5Core.dll" );
module:add_binary_file( "Qt5Widgets.dll" );
module:add_binary_file( "Qt5Gui.dll" );

module.ui_files = {}
module.moc_files = {}


module.import_func = function(project)
	local output_path = path.getabsolute( _OPTIONS[ "qt_dir" ] )
	
	module:add_include_dir( output_path );
		
	local uic_exe = path.join( qtDir, "bin\\uic.exe" );
	local moc_exe = path.join( qtDir, "bin\\moc.exe" );
	
	local moc_defines = "-D_WINDOWS -DUNICODE -DWIN32 -DWIN64 -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB"
	
	local moc_include_dirs = {
		path.join( qtDir, "include" ),
		path.join( qtDir, "include\\QtWidgets" ),
		path.join( qtDir, "include\\QtGui" ),
		path.join( qtDir, "include\\QtCore" )
	};
	
	local moc_includes = "-I" .. table.concat( moc_include_dirs," -I" );
	
	for i,file_name in pairs( module.ui_files ) do
		local output_file = path.join( output_path, "ui_" .. path.getbasename( file_name ) .. ".h" )
		local input_file = path.getabsolute( file_name )
		local command_line = uic_exe .. " -o \"" .. output_file .. "\" \"" .. input_file .. "\""
		
		print( "UI: " .. file_name );
		prebuildcommands{ command_line }
		os.execute( command_line );
		
		module:add_files( output_file );
	end
	
	for i,file_name in pairs( module.moc_files ) do
		local output_file = path.join( output_path, "moc_" .. path.getbasename( file_name ) .. ".cpp" )
		local input_file = path.getabsolute( file_name )
		local command_line = moc_exe .. " \"" .. input_file .. "\" -o \"" .. output_file .. "\" " .. moc_defines .. " " .. moc_includes

		print( "Moc: " .. file_name );
		prebuildcommands{ command_line }
		os.execute( command_line );
		
		module:add_files( output_file );
	end
end
