
newoption{ trigger = "script", description = "Script to load" };
newoption{ trigger = "outpath", description = "Location for generated project files" };

if not _OPTIONS["outpath"] then
	error("No outpath specified.")
end

if not _OPTIONS["script"] then
	error("No script specified.")
end

global_configuration = {
	scripts_path = path.getabsolute( path.getdirectory( _SCRIPT ) ),
	root_path = path.getabsolute( path.join( path.getdirectory( _SCRIPT ), "../../../" ) ),
	output_path = path.getrelative( _OPTIONS["outpath"], "." )
}

dofile( path.join( global_configuration.scripts_path, "thirdparty/datadumper.lua" ) );
dofile( path.join( global_configuration.scripts_path, "global.lua" ) );

function executeBuildSteps()
	local scriptFile = path.join( global_configuration.output_path, "build_scripts", _OPTIONS[ "script" ] ) .. ".lua";
	if not os.isfile( scriptFile )	then
		throw( "Script file not found at " .. scriptFile );
	end
	
	local buildActions = dofile( scriptFile );
	for i, action in pairs( buildActions ) do
		if not os.isfile( action.script )	then
			throw( "Action script file not found at " .. action.script );
		end
	
		local actionFunction = dofile( action.script );
		actionFunction( action.data );
	end
end

newaction {
   trigger     = "buildsteps",
   description = "Execute Build Steps",
   execute     = executeBuildSteps
}
