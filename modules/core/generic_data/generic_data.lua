local module = Module:new( "generic_data" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( path.join( global_configuration.root_path, "content/core/generic_data/types/*.*" ) );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "core/base" );
module:add_dependency( "core/container" );
module:add_dependency( "core/resource" );

module.import_func = function( project )
	local output_path = path.getabsolute( _OPTIONS[ "generated_files_dir" ] );
	
	module:add_include_dir( output_path );
	files( { path.join( output_path, "*.hpp" ), path.join( output_path, "*.cpp" ) } );
	
	local exe_name = iff(is_windows, "generic_data_code_generator.exe", "generic_data_code_generator");
	local exe_path = path.join( global_configuration.root_path, "library/buildtools/generic_data_code_generator/" .. exe_name );
	local content_path = path.join( global_configuration.root_path, "content" );
	local arguments = "--content-dir=" .. content_path .. " --target-dir=" .. output_path;
	local command_line = exe_path .. " " .. arguments;

	print( command_line );
	prebuildcommands { command_line };
	
	os.execute( command_line );
end

