
Module = class{ name = nil, config = nil, module_dependencies = {}, source_files = {} };

global_module_storage = {};

function find_module( module_name )
	for i,module in pairs( global_module_storage ) do
		if ( module.name == module_name ) then
			return module;
		end
	end

	throw( "[find_module] Module with name '"..module_name.."' not found." );
	return nil;
end

function Module:new( name, initFunc )
	local module_new = class_instance( self );
	module_new.name		= name;
	module_new.config	= PlatformConfiguration:new();
	
	table.insert( global_module_storage, module_new );

	if ( initFunc ~= nil and type( initFunc ) == "function" ) then
		initFunc( module_new );
	end

	return module_new;
end

function Module:add_files( file_name )
	table.insert( self.source_files, path.getabsolute( file_name ) );
end

function Module:set_define( name, value, configuration, platform )
	self.config:set_define( name, value, configuration, platform );
end

function Module:set_flag( name, configuration, platform )
	self.config:set_flag( name, configuration, platform );
end

function Module:add_shader_dir( shader_dir, configuration, platform )
	self.config:add_shader_dir( shader_dir, configuration, platform );
end

function Module:add_binary_dir( binary_dir, configuration, platform )
	self.config:add_binary_dir( binary_dir, configuration, platform );
end

function Module:add_include_dir( include_dir, configuration, platform )
	self.config:add_include_dir( include_dir, configuration, platform );
end

function Module:add_library_dir( library_dir, configuration, platform )
	self.config:add_library_dir( library_dir, configuration, platform );
end

function Module:add_binary_file( binary_filename, configuration, platform )
	self.config:add_binary_file( binary_filename, configuration, platform );
end

function Module:add_library_file( library_filename, configuration, platform )
	self.config:add_library_file( library_filename, configuration, platform );
end

function Module:add_dependency( module_name )
	if not type( module_name ) == "string" then
		throw( "[Module:add_dependency] module_name is not a valid string." );		
	end

	table.insert( self.module_dependencies, module_name );
end

function Module:resolve_dependency( target_list )
	for i,module_name in pairs( self.module_dependencies ) do
		local module = find_module( module_name )

		if not table.contains( target_list, module ) then
			table.insert( target_list, module );
			module:resolve_dependency( target_list );
		end		
	end
end

function Module:finalize( shader_dirs, binary_dirs, binary_files, configuration, platform )
	if ( configuration == nil and platform == nil ) then
		files( self.source_files );

		--[[for i,file in pairs( self.source_files ) do
			print( "File: "..file );
		end]]--
	end

	self.config:get_config( configuration, platform ):apply( shader_dirs, binary_dirs, binary_files );
end