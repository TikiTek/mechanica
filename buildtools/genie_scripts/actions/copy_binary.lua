-- library/buildtools/genie_scripts/actions/copy_binary
function copy_binary( data )
	if not os.isfile( data.source )
		throw( "[copy_binary] Source file not found at " .. data.source )
	end
	
	if os.isfile( data.target )
	else
		os.copyfile( data.source, data.target );
	end
	local source_file = data.source;
	local 
end