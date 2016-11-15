-- library/buildtools/genie_scripts/extensions/actions/qt_moc
return function( data, config )
	local output_file = path.join( config.output_path, data.output_filename );
	
	if is_build_required( data.source_filename, output_file ) then
		local rcc_exe = path.join( data.qt_dir, "bin\\rcc.exe" );
		local command_line = rcc_exe .. " -binary \"" .. data.source_filename .. "\" -o \"" .. output_file .. "\"";
		
		print( "RCC: " .. path.getname( data.source_filename ) );
		if not os.execute( command_line ) then
			throw( "rcc return an error" );
		end
	end
end