-- library/buildtools/genie_scripts/extensions/actions/qt_moc
return function( data, config )
	local output_path = path.join( config.build_path, "qt_files", config.project_name );
	local output_file = path.join( output_path, data.output_filename )
	
	local copy = false;
	if os.isfile( output_file ) then
		if os.stat( data.source_filename ).mtime > os.stat( output_file ).mtime then
			copy = true;
		end
	else
		copy = true;
	end
	
	if copy then
		local moc_exe = path.join( data.qt_dir, "bin\\moc.exe" );
		local moc_defines = "-D_WINDOWS -DUNICODE -DWIN32 -DWIN64 -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB"
		
		local moc_include_dirs = {
			path.join( data.qt_dir, "include" ),
			path.join( data.qt_dir, "include\\QtWidgets" ),
			path.join( data.qt_dir, "include\\QtGui" ),
			path.join( data.qt_dir, "include\\QtCore" )
		};
		
		local moc_includes = "-I" .. table.concat( moc_include_dirs," -I" );

		print( "Moc: " .. path.getname( data.source_filename ) );

		local command_line = moc_exe .. " \"" .. data.source_filename .. "\" -o \"" .. output_file .. "\" " .. moc_defines .. " " .. moc_includes
		os.execute( command_line );
	end
end