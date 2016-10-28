
newoption { trigger = "unity_dir", description = "" }
newoption { trigger = "genericdata_dir", description = "" }
newoption { trigger = "qt_dir", description = "" }

Solution = class{
	name = nil,
	projects = {}
};

function add_extension( name )
	local script_path = path.join( global_configuration.scripts_path, "extensions/extension." .. name .. ".lua" );
	dofile( script_path );
end

function Solution:new( name )
	if not name then 
		throw( "[Project:new] No name given." );
	end
	
	local solution_new = class_instance( self );
	solution_new.name = name;
	
	return solution_new;
end

function Solution:add_project( project )
	if type( project ) ~= "table" then
		throw "[Solution:add_project] project argument is invalid.";
	end

	table.insert( self.projects, project );
end

function Solution:finalize()
	local var_platforms = {};
	local var_configurations = {};

	for i,project in pairs( self.projects ) do
		for i,platform in pairs( project.platforms ) do
			if not table.contains( var_platforms, platform ) then 
				table.insert( var_platforms, platform );
			end
		end
		for j,configuration in pairs( project.configurations ) do
			if not table.contains( var_configurations, configuration ) then 
				table.insert( var_configurations, configuration );
			end
		end
	end
	table.insert( var_configurations, 'Project' );
	
	solution( self.name );
	configurations( var_configurations );
	platforms( var_platforms );
	location( _OPTIONS[ "outpath" ] )

	toolchain( _OPTIONS[ "outpath" ], "" );

	for i,project in pairs( self.projects ) do
		print( "Project: " .. project.name );

		_OPTIONS[ "unity_dir" ] = path.join( _OPTIONS[ "outpath" ], "unity_files", project.name )
		if not os.isdir( _OPTIONS[ "unity_dir" ] ) then
			os.mkdir( _OPTIONS[ "unity_dir" ] )
		end

		_OPTIONS[ "genericdata_dir" ] = path.join( _OPTIONS[ "outpath" ], "genericdata_files", project.name )
		if not os.isdir( _OPTIONS[ "genericdata_dir" ] ) then
			os.mkdir( _OPTIONS[ "genericdata_dir" ] )
		end

		_OPTIONS[ "qt_dir" ] = path.join( _OPTIONS[ "outpath" ], "qt_files", project.name )
		if not os.isdir( _OPTIONS[ "qt_dir" ] ) then
			os.mkdir( _OPTIONS[ "qt_dir" ] )
		end
		
		project:finalize_project();
	end
	
	--local premake_path = global_configuration.root_path .. '\\library\\buildtools\\premake\\premake5.exe'	;
	--filter( 'Project' );
	--kind( 'Makefile' );
	--buildcommands( { 'cd ..\\project', premake_path .. ' /outpath=../build ' .. _ACTION } );
end