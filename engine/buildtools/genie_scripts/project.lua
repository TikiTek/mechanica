
ProjectTypes = {
	consoleApplication	= "ConsoleApp",
	windowApplication	= "WindowedApp",
	sharedLibrary		= "SharedLib",
	staticLibrary		= "StaticLib"
};

ProjectLanguages = {
	cpp		= "C++",
	cs		= "C#"
};

Project = class{
	name = nil,
	uuid = nil,
	type = nil,
	lang = ProjectLanguages.cpp,
	module = nil,
	buildoptions = nil,
	config = nil,
	platforms = {},
	configurations = {}
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

function Project:new( name, uuid, platforms, configurations, module, projectType )
	if not name then 
		throw( "[Project:new] No name given." );
	end

	if ( type( platforms ) ~= "table" or type( configurations ) ~= "table" ) then 
		throw( "[Project:new] platforms or configurations are invalid." );
	end

	if not module then 
		throw( "[Project:new] No module given." );
	end

	if not projectType then 
		throw( "[Project:new] type is invalid." );
	end

	local project_new = class_instance( self );
	project_new.name			= name;
	project_new.uuid			= uuid;
	project_new.type			= projectType;
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

function Project:finalize_binary( config, target_path )
	for i,file in pairs( config.binary_files ) do
		for j,dir in pairs( config.binary_dirs ) do
			local fullpath = path.join( dir, file );

			if os.isfile( fullpath ) then
				local step_script = path.join( global_configuration.scripts_path, "actions/copy_binary.lua" );
				local step_data = {
					source = fullpath,
					target = path.join( target_path, file )
				};
				
				config:add_post_build_step( step_script, step_data );
				break;
			end
		end
	end
end

function Project:finalize_config( config )
	local array_defines = table.sort( table.uniq( config.defines ) );
	local array_flags = table.sort( table.uniq( config.flags ) );
	local array_include_dirs = table.sort( table.uniq( config.include_dirs ) );
	local array_library_dirs = table.sort( table.uniq( config.library_dirs ) );
	local array_library_files = table.sort( table.uniq( config.include_dirs ) );

	if array_defines then
		defines( array_defines );
	end
	
	if array_flags then
		flags( array_flags );
	end
	
	if array_include_dirs then
		includedirs( array_include_dirs );
	end
	
	if array_library_dirs then
		libdirs( array_library_dirs );
	end
	
	if array_library_files then
		links( array_library_files );
	end
end

function Project:finalize_build_steps( configuration, target_path, configuration, platform )

end

function Project:finalize_project()
	project( self.name )
	uuid( self.uuid );
	kind( self.type );
	language( self.lang );
	
	if self.buildoptions then
		buildoptions( self.buildoptions );
	end
	
	local config_global = Configuration:new();
	if self.lang == ProjectLanguages.cpp then
		config_global:set_define( "TIKI_PROJECT_NAME", self.name );

		if self.type == ProjectTypes.sharedLibrary or self.type == ProjectTypes.staticLibrary then
			config_global:set_define( "TIKI_BUILD_LIBRARY", "TIKI_ON" );
		else
			config_global:set_define( "TIKI_BUILD_LIBRARY", "TIKI_OFF" );
		end
	end

	local modules = {};
	self.module:resolve_dependency( modules );

	self.module:finalize_module( config_global, nil, nil, self );
	for _,cur_module in pairs( modules ) do
		cur_module:finalize_module( config_global, nil, nil, self );
	end

	local config_platform = {};
	for _,build_platform in pairs( self.platforms ) do
		--print( "Platform: " .. build_platform );
		configuration{ build_platform };

		config_platform[ build_platform ] = Configuration:new()

		self.module:finalize_module( config_platform[ build_platform ], nil, build_platform, self );
		for j,cur_module in pairs( modules ) do
			cur_module:finalize_module( config_platform[ build_platform ], nil, build_platform, self );
		end

		local project_config = self.config:get_config( nil, build_platform );
		if project_config then
			project_config:apply_configuration( config_platform[ build_platform ] );
		end
	end

	local config_configuration = {};
	for _,build_config in pairs( self.configurations ) do
		--print( "Configuration: " .. build_config );
		configuration{ build_config };

		config_configuration[ build_config ] = Configuration:new();

		self.module:finalize_module( config_configuration[ build_config ], build_config, nil, self );
		for j,cur_module in pairs( modules ) do
			cur_module:finalize_module( config_configuration[ build_config ], build_config, nil, self );
		end

		local project_config = self.config:get_config( build_config, nil );
		if project_config then
			project_config:apply_configuration( config_configuration[ build_config ] );
		end
	end

	for _,build_platform in pairs( self.platforms ) do
		for j,build_config in pairs( self.configurations ) do
			--print( "Configuration: " .. build_platform .. "/" .. build_config );
			configuration{ build_config, platform };

			local target_path = get_config_dir( build_platform, build_config );
			targetdir( target_path );
			debugdir( target_path );
			objdir( path.join( target_path, "obj" ) );
			
			local config = Configuration:new();

			self.module:finalize_module( config, build_config, build_platform, self );
			for k,cur_module in pairs( modules ) do
				cur_module:finalize_module( config, build_config, build_platform, self );
			end	
			
			local project_config = self.config:get_config( build_config, build_platform );
			if project_config then
				project_config:apply_configuration( config );
			end	
			
			config_global:apply_configuration( config );
			config_platform[ build_platform ]:apply_configuration( config );
			config_configuration[ build_config ]:apply_configuration( config );
			
			self:finalize_binary( config, target_path );
			self:finalize_config( config );
			self:finalize_build_steps( config, target_path, build_config, build_platform );
		end
	end

	local shader_file_name = _OPTIONS[ "outpath" ] .. "/shaderinc.lst";
	local shader_lines = {};
	if io.exists( shader_file_name ) then
		for line in io.lines( shader_file_name ) do 
			shader_lines[ #shader_lines + 1 ] = line
		end	
	end
	for _,dir in pairs( config_global.shader_dirs ) do
		if table.index_of( shader_lines, dir ) == -1 then
			table.insert( shader_lines, dir );
		end
	end
	
	local shader_file = io.open( shader_file_name, "w" );
	if shader_file ~= nil then		
		for i,dir in pairs( shader_lines ) do
			--print( "Shader: " .. dir );

			if i > 1 then
				shader_file:write( "\n" );
			end

			shader_file:write( dir );
		end
		shader_file:close();
	end
end