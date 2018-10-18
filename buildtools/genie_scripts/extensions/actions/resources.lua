return function( data, config )
	local matches = os.matchfiles( data.input_pattern );
	if #matches == 0 then
		throw("[resource] '" .. data.input_pattern .. "' pattern matches no files.");
	end

	local output_path = path.join( config.build_path, "generated_files", config.project_name );
	local output_file = path.join( output_path, 'res_' .. data.output_name .. '.hpp' );
	
	local needBuild = false
	for i, filename in pairs( matches ) do
		needBuild = needBuild or is_build_required( filename, output_file );
	end
	
	if needBuild then
		local command_line = data.tool_path .. " --output-dir=\"" .. output_path .. "\" --output-name=" .. data.output_name;
		for i, filename in pairs( matches ) do
			command_line = command_line .. " \"" .. filename .. "\""
		end
		
		print( "Resource: " .. data.output_name );
		if not os.execute( command_line ) then
			throw( "resource returned an error. command line: " .. command_line );
		end
	end
end