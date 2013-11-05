
Project = class{ name = nul, type = nil, module = nil, config = nil, platforms = {}, configurations = {} };

ProjectTypes = {
	consoleApplication	= "ConsoleApp",
	windowApplication	= "WindowedApp",
	sharedLibrary		= "SharedLib",
	staticLibrary		= "StaticLib"
};

global_project_storage = {};

function find_project( project_name )
	for i,project in pairs( global_project_storage ) do
		if ( project.name == project_name ) then
			return project;
		end
	end

	throw( "[find_project] Project with name '"..project_name.."' not found." );
	return nil;
end

function Project:new( name, platforms, configurations, module, type2 )
	if not name then 
		throw( "[Project:new] No name given." );
	end

	if ( type( platforms ) ~= "table" or type( configurations ) ~= "table" ) then 
		throw( "[Project:new] platforms or configurations are invalid." );
	end

	if not module then 
		throw( "[Project:new] No module given." );
	end

	if not type2 then 
		throw( "[Project:new] type is invalid." );
	end

	local project_new = class_instance( self );
	project_new.name			= name;
	project_new.type			= type2;
	project_new.config			= PlatformConfiguration:new();
	project_new.module			= module;
	project_new.configurations	= configurations;
	project_new.platforms		= platforms;

	table.insert( global_project_storage, project_new );

	return project_new;
end

function Project:set_define( name, value, configuration, platform )
	self.config:set_define( name, value, configuration, platform );
end

function Project:set_flag( name, configuration, platform )
	self.config:set_flag( name, configuration, platform );
end

function Project:add_shader_dir( shader_dir, configuration, platform )
	self.config:add_shader_dir( shader_dir, configuration, platform );
end

function Project:add_binary_dir( binary_dir, configuration, platform )
	self.config:add_binary_dir( binary_dir, configuration, platform );
end

function Project:add_include_dir( include_dir, configuration, platform )
	self.config:add_include_dir( include_dir, configuration, platform );
end

function Project:add_library_dir( library_dir, configuration, platform )
	self.config:add_library_dir( library_dir, configuration, platform );
end

function Project:add_binary_file( binary_filename, configuration, platform )
	self.config:add_binary_file( binary_filename, configuration, platform );
end

function Project:add_library_file( library_filename, configuration, platform )
	self.config:add_library_file( library_filename, configuration, platform );
end

function Project:finalize_binary( binary_dirs, binary_files )
	local dirs = table.join_array( {}, binary_dirs );
	local files = table.join_array( {}, binary_files );

	for i,file in pairs( files ) do
		--print( "File: " .. file );

		for j,dir in pairs( dirs ) do
			local fullpath = path.join( dir, file );

			if os.isfile( fullpath ) then
				--print( "Binary: " .. fullpath  );

				fullpath = string.gsub( fullpath, "/", "\\" );
				postbuildcommands{ "copy \"" .. fullpath .. "\" \"$(TargetDir)" .. file .. "\"" };
			end
		end
	end
end

function Project:finalize()
	project( self.name )
	kind( self.type );
	language "C++"

	local modules = {};
	local shader_global_dirs = {};
	local binary_global_dirs = {};
	local binary_global_files = {};
	self.module:resolve_dependency( modules );

	self.module:finalize( shader_global_dirs, binary_global_dirs, binary_global_files );
	for i,cur_module in pairs( modules ) do
		--print( "Module: "..cur_module.name );
		cur_module:finalize( shader_global_dirs, binary_global_dirs, binary_global_files );
	end
	self:finalize_binary( { binary_global_dirs }, { binary_global_files } );

	local binary_platform_dirs = {};
	local binary_platform_files = {};
	for i,var_platform in pairs( self.platforms ) do
		configuration{ var_platform };
		--print( "Platform: " .. var_platform );

		binary_platform_dirs[ var_platform ] = {};
		binary_platform_files[ var_platform ] = {};

		self.module:finalize( nil, binary_platform_dirs[ var_platform ], binary_platform_files[ var_platform ], nil, var_platform );

		for j,cur_module in pairs( modules ) do
			cur_module:finalize( nil, binary_platform_dirs[ var_platform ], binary_platform_files[ var_platform ], nil, var_platform );
		end

		local project_config = self.config:get_config( nil, var_platform );
		if project_config then
			project_config:apply( nil, binary_platform_dirs[ var_platform ], binary_platform_files[ var_platform ] );
		end

		self:finalize_binary( { binary_global_dirs, binary_platform_dirs[ var_platform ] }, { binary_platform_files[ var_platform ] } );
	end

	local binary_configuration_dirs = {};
	local binary_configuration_files = {};
	for i,var_configuration in pairs( self.configurations ) do
		configuration{ var_configuration };
		--print( "Configuration: " .. var_configuration );

		binary_configuration_dirs[ var_configuration ] = {};
		binary_configuration_files[ var_configuration ] = {};

		self.module:finalize( nil, binary_configuration_dirs[ var_configuration ], binary_configuration_files[ var_configuration ], var_configuration );

		for j,cur_module in pairs( modules ) do
			cur_module:finalize( nil, binary_configuration_dirs[ var_configuration ], binary_configuration_files[ var_configuration ], var_configuration );
		end

		local project_config = self.config:get_config( var_configuration );
		if project_config then
			project_config:apply( nil, binary_configuration_dirs[ var_configuration ], binary_configuration_files[ var_configuration ] );
		end

		self:finalize_binary( { binary_global_dirs, binary_configuration_dirs[ var_configuration ] }, { binary_configuration_files[ var_configuration ] } );
	end

	local binary_pc_dirs = {};
	local binary_pc_files = {};
	for i,var_platform in pairs( self.platforms ) do
		binary_pc_dirs[ var_platform ] = {};
		binary_pc_files[ var_platform ] = {};

		for j,var_configuration in pairs( self.configurations ) do
			configuration{ var_configuration, var_platform };

			local path = get_config_dir( var_platform, var_configuration );
			targetdir( path );
			debugdir( path );
			objdir( path .. "/obj" );

			print( "Configuration: " .. var_platform .. "/" .. var_configuration );

			binary_pc_dirs[ var_platform ][ var_configuration ] = {};
			binary_pc_files[ var_platform ][ var_configuration ] = {};
			
			self.module:finalize( nil, binary_pc_dirs[ var_platform ][ var_configuration ], binary_pc_files[ var_platform ][ var_configuration ], var_configuration, var_platform );

			for k,cur_module in pairs( modules ) do
				cur_module:finalize( nil, binary_pc_dirs[ var_platform ][ var_configuration ], binary_pc_files[ var_platform ][ var_configuration ], var_configuration, var_platform );
			end	
			
			local project_config = self.config:get_config( var_configuration, var_platform );
			if project_config then
				project_config:apply( nil, binary_pc_dirs[ var_platform ][ var_configuration ], binary_pc_files[ var_platform ][ var_configuration ] );
			end	
			
			self:finalize_binary( { binary_global_dirs, binary_platform_dirs[ var_platform ], binary_configuration_dirs[ var_configuration ], binary_pc_dirs[ var_platform ][ var_configuration ] }, { binary_platform_files[ var_platform ], binary_configuration_files[ var_configuration ], binary_pc_files[ var_platform ][ var_configuration ] } );
		end
	end

	local shader_file = io.open( _OPTIONS[ "outpath" ] .. "/shaderinc.lst", "w" );
	if shader_file ~= nil then
		for i,dir in pairs( shader_global_dirs ) do
			if i > 1 then
				shader_file:write( "\n" );
			end

			shader_file:write( dir );
		end
		shader_file:close();
	end
end