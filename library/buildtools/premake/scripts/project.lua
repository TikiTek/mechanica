
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

	error( "[find_project] Project with name '"..project_name.."' not found." );
	return nil;
end

function Project:new( name, platforms, configurations, module, type )
	if not ( name and platforms and configurations and module and type ) then
		error("Not enough arguments.")
	end

	local project_new = class_instance( self );
	project_new.name			= name;
	project_new.type			= type;
	project_new.config			= PlatformConfiguration:new();
	project_new.module			= module;
	project_new.configurations	= configurations;

	for i,platform in pairs( platforms ) do
		project_new.platforms[ platform ] = Platform:new( platform, configurations );
	end

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

	for i,configuration in pairs( self.configurations ) do
		self.module:finalize( configuration );

		for j,cur_module in pairs( modules ) do
			cur_module:finalize( configuration );
		end

		local project_config = self.config:get_config( configuration );
		if project_config then
			project_config:apply();
		end
	end

	for platform_name,platform in pairs( self.platforms ) do
		for j,configuration in pairs( self.configurations ) do
			self.module:finalize( configuration, platform_name );

			for k,cur_module in pairs( modules ) do
				cur_module:finalize( configuration, platform_name );
			end	
			
			local project_config = self.config:get_config( configuration, platform_name );
			if project_config then
				project_config:apply();
			end				
		end
	end

	defines { "DEBUG", "TIKI_ON=2", "TIKI_OFF=1", "TIKI_CURRENT_MODULE=PENIS" }
	   
	configuration "x32"
	defines { "TIKI_BUILD_32BIT=TIKI_ON", "TIKI_BUILD_64BIT=TIKI_OFF", "TIKI_PLATFORM_WIN=TIKI_ON", "TIKI_PLATFORM_LINUX=TIKI_OFF" }
 
	configuration "Debug"
	defines { "DEBUG", "TIKI_BUILD_DEBUG=TIKI_ON" }
	flags { "Symbols" }
 
	configuration "Release"
	defines { "NDEBUG", "TIKI_BUILD_RELEASE=TIKI_ON" }
	flags { "Optimize" }

	configuration "Master"
	defines { "NDEBUG", "TIKI_BUILD_MASTER=TIKI_ON" }
	flags { "Optimize" }
end