
newoption{ trigger = "script", description = "Script to load" };
newoption{ trigger = "outpath", description = "Location for generated project files" };
newoption{ trigger = "project", description = "Name of the Project" };

if not _OPTIONS["outpath"] then
	error("No outpath specified.")
end

if not _OPTIONS["script"] then
	error("No script specified.")
end

global_configuration = {
	scripts_path = path.getabsolute( path.getdirectory( _SCRIPT ) ),
	root_path = path.getabsolute( path.join( path.getdirectory( _SCRIPT ), "../../../" ) )
}

dofile( path.join( global_configuration.scripts_path, "thirdparty/datadumper.lua" ) );
dofile( path.join( global_configuration.scripts_path, "global.lua" ) );

function executeBuildSteps()
	local config = {
		project_name = _OPTIONS[ "project" ],
		build_path = path.getabsolute( "." ),
		output_path = path.getabsolute( _OPTIONS["outpath"] )		
	};

	local script_file = path.join( config.build_path, _OPTIONS[ "script" ] );
	if not os.isfile( script_file )	then
		throw( "Script file not found at " .. script_file );
	end
	
	local build_actions = dofile( script_file );
	for i, action in pairs( build_actions ) do
		if not os.isfile( action.script )	then
			throw( "Action script file not found at " .. action.script );
		end
	
		local action_function = dofile( action.script );
		if type( action_function ) ~= "function" then
			throw( "Script in " .. action.script .. " doesn't contain a function." );
		end
		
		action_function( action.data, config );
	end
end

newaction {
   trigger     = "buildsteps",
   description = "Execute Build Steps",
   execute     = executeBuildSteps
}
