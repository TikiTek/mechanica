
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

	local project_new = setmetatable( {}, self );
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
	self.config.set_define( name, value, configuration, platform );
end

function Project:set_flag( name, configuration, platform )
	self.config.set_flag( name, configuration, platform );
end

function Project:finalize()
	project( self.name )
	kind( self.type );
	language "C++"

	local modules = {};
	self.module:resolve_dependency( modules );

	for i,module in pairs( modules ) do
		module:finalize();
	end
   
	--files { "include/**/*.hpp", "source/*.cpp" }
 
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