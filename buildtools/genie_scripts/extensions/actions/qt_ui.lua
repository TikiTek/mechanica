-- library/buildtools/genie_scripts/extensions/actions/qt_ui
return function( data, config )
	local uic_exe = path.join( data.qt_dir, "bin\\uic.exe" );
	local output_path = path.join( config.build_path, "qt_files", config.project_name );

	print( "UI: " .. path.getname( data.source_filename ) );

	local command_line = uic_exe .. " -o \"" .. data.output_filename .. "\" \"" .. data.source_filename .. "\""
	os.execute( command_line );
end