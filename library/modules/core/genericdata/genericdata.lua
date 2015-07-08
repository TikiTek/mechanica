-- library/modules/runtime/genericdata

local module = Module:new( "genericdata" );


module.import_func = function()
	local output_path = path.getabsolute( _OPTIONS[ "genericdata_dir" ] );

	module:add_include_dir( output_path );
	module:add_files( path.join( output_path, "*.hpp" ) );
	
	local exe_path = path.join( global_configuration.root_path, "library/buildtools/genericdatacodegenerator/genericdatacodegenerator.exe" );
	local content_path = path.join( global_configuration.root_path, "content" );
	local arguments = "--content-dir=" .. content_path .. " --target-dir=" .. output_path;

	prebuildcommands { exe_path .. " " .. arguments };
	
	print( "Using GenericData" );
end

