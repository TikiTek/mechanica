local module = Module:new( "generic_data" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( path.join( tiki.project_path, "content/core/generic_data/types/*.*" ) );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "core/base" );
module:add_dependency( "core/container" );
module:add_dependency( "core/resource" );

module.import_func = function( project )
	local output_path = path.getabsolute( project.generated_files_dir );

	module:add_include_dir( output_path );
	files( { path.join( output_path, "*.hpp" ), path.join( output_path, "*.cpp" ) } );

	local exe_name = iff( tiki.target_platform == Platforms.Windows, ".exe", "" );
	local exe_path = path.join( tiki.project_path, "engine/buildtools/generic_data_code_generator/generic_data_code_generator" .. exe_name );
	local content_path = path.join( tiki.project_path, "content" );
	local arguments = "--content-dir=" .. content_path .. " --target-dir=" .. output_path;
	local command_line = exe_path .. " " .. arguments;

	prebuildcommands { command_line };

	if _ACTION ~= "targets" then
		if not os.execute( command_line ) then
			throw( "Failed to generate generic Data header. Command line: " .. command_line )
		end
	end
end

