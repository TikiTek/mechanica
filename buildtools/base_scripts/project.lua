
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

function Project:new( name, platforms, configurations, module, type )
	if not ( name and platforms and configurations and module and type ) then
		throw("Not enough arguments.")
	end

	local project_new = class_instance( self );
	project_new.name			= name;
	project_new.type			= type;
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

function Project:finalize()
	project( self.name )
	kind( self.type );
	language "C++"

	local modules = {};
	self.module:resolve_dependency( modules );

	self.module:finalize();
	for i,cur_module in pairs( modules ) do
		--print( "Module: "..cur_module.name );
		cur_module:finalize();
	end

	for i,var_configuration in pairs( self.configurations ) do
		configuration{ var_configuration };
		print( "Configuration: " .. var_configuration );

		self.module:finalize( var_configuration );

		for j,cur_module in pairs( modules ) do
			cur_module:finalize( var_configuration );
		end

		local project_config = self.config:get_config( var_configuration );
		if project_config then
			project_config:apply();
		end
	end

	for i,var_platform in pairs( self.platforms ) do
		for j,var_configuration in pairs( self.configurations ) do
			configuration{ var_configuration, var_platform };
			print( "Configuration: " .. var_configuration .. "/" .. var_platform );

			self.module:finalize( var_configuration, var_platform );

			for k,cur_module in pairs( modules ) do
				cur_module:finalize( var_configuration, var_platform );
			end	
			
			local project_config = self.config:get_config( var_configuration, var_platform );
			if project_config then
				project_config:apply();
			end				
		end
	end
end