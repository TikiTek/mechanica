-- library/buildtools/genie_scripts/actions/copy_binary
return function( data, config )
	if not os.isfile( data.source ) then
		throw( "[copy_binary] Source file not found at " .. data.source )
	end
	
	local target_path = path.join( config.output_path, data.target );
	
	local copy = false;
	if os.isfile( target_path ) then
		if os.stat( data.source ).mtime > os.stat( target_path ).mtime then
			copy = true;
		end
	else
		copy = true;
	end
	
	if copy then
		print( "Copy " .. data.source .. " to output directory" );
		
		
		os.copyfile( data.source, target_path );
	end
end