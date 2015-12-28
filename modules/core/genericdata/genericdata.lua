-- library/modules/runtime/genericdata

local module = Module:new( "genericdata" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( path.join( global_configuration.root_path, "content/genericdata/types/*.*" ) );
module:add_files( "genericdata.lua" );
module:add_include_dir( "include" );

module:add_dependency( "resource" );

module.import_func = function(project)
	local output_path = path.getabsolute( _OPTIONS[ "genericdata_dir" ] );
	
	includedirs( { output_path } );
	files( { path.join( output_path, "*.hpp" ), path.join( output_path, "*.cpp" ) } );
	
	local exe_path = path.join( global_configuration.root_path, "library/buildtools/genericdatacodegenerator/genericdatacodegenerator.exe" );
	local content_path = path.join( global_configuration.root_path, "content" );
	local arguments = "--content-dir=" .. content_path .. " --target-dir=" .. output_path;
	local command_line = exe_path .. " " .. arguments;

	prebuildcommands { command_line };
	
	print( "Using GenericData" );
	os.execute( command_line );
end

