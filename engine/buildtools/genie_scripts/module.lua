
Module = class{
	name = nil,
	module_type = 0,
	import_func = nil,
	config = nil,
	module_dependencies = {},
	source_files = {},
	exclude_files = {},
	stack_trace = ""
};

global_module_storage = {};

ModuleTypes = {
	UnityCppModule	= 0,
	UnityCModule	= 1,
	FilesModule		= 2
};

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
	for i,module in pairs( global_module_storage ) do
		if ( module.name == name ) then
			throw( "Module name already used: " .. name .. "\n" .. module.stack_trace );
		end
	end

	local module_new = class_instance( self );
	module_new.name			= name;
	module_new.config		= PlatformConfiguration:new();
	module_new.module_type	= ModuleTypes.UnityCppModule;
	module_new.stack_trace	= debug.traceback();
		
	table.insert( global_module_storage, module_new );

	if ( initFunc ~= nil and type( initFunc ) == "function" ) then
		initFunc( module_new );
	end

	--print( "Module: "..module_new.name );
	
	return module_new;
end

function Module:set_base_path( base_path )
	self.config:set_base_path( base_path );
end

function Module:add_files( file_name, flags )
	local target_list = self.source_files;

	if type( flags ) == "table" then
		if flags.exclude then
			target_list = self.exclude_files;
		end
	end

	table.insert( target_list, path.join( self.config.base_path, file_name ) );
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
	print( "Module: "..self.name );

	for i,module_name in pairs( self.module_dependencies ) do
		local module = find_module( module_name )

		if not table.contains( target_list, module ) then
			table.insert( target_list, module );
			module:resolve_dependency( target_list );
		end		
	end
end

function Module:finalize( shader_dirs, binary_dirs, binary_files, configuration_obj, platform, project )
	if ( configuration_obj == nil and platform == nil ) then
		if self.import_func ~= nil and type( self.import_func ) == "function" then
			self.import_func(project);
		end
		
		local all_files = {};
	
		for i,pattern in pairs( self.source_files ) do
			local matches = os.matchfiles( pattern )
			
			if #matches == 0 then
				throw("[finalize] '" .. pattern .. "' pattern in '" .. self.name .. "' matches no files.");
			end
			
			for j,file_name in pairs( matches ) do
				if not io.exists( file_name ) then
					throw("[finalize] '" .. file_name .. "'  in '" .. self.name .. "' don't exists.");
				end
				
				if not table.contains( all_files, file_name ) then
					all_files[#all_files+1] = file_name;
				end					
			end
		end
		
		for i,pattern in pairs( self.exclude_files ) do
			local matches = os.matchfiles( pattern )
			
			for j,file_name in pairs( matches ) do
				local index = table.index_of( all_files, file_name );
				
				while index >= 0 do
					table.remove( all_files, index );
				
					index = table.index_of( all_files, file_name );
				end
			end
		end

		if global_configuration.enable_unity_builds and ( self.module_type == ModuleTypes.UnityCppModule or self.module_type == ModuleTypes.UnityCModule ) then
			local ext = "cpp"
			if self.module_type == ModuleTypes.UnityCModule then
				ext = "c"
			end
			
			local unity_file_name = path.join( _OPTIONS[ "unity_dir" ], self.name .. "_unity." .. ext );			
			local c = {};
			c[#c+1] = "// Unity file created by GENie";
			c[#c+1] = "";
			c[#c+1] = "#define TIKI_CURRENT_MODULE \"" .. self.name .. "\"";
			c[#c+1] = "";
			for i,file_name in pairs( all_files ) do
				if path.iscppfile( file_name ) then
					file_action( path.getrelative( _OPTIONS[ "outpath" ], file_name ), "Header" );
					
					local relative_file_name = path.getrelative( _OPTIONS[ "unity_dir" ], file_name );
					c[#c+1] = string.format( "#include \"%s\"", relative_file_name );
				end
			end
			local unity_content = table.concat( c, "\n" );

			files( all_files );
			excludes( all_files );
			
			local create_unity = true
			if os.isfile( unity_file_name ) then
				local unity_file = io.open( unity_file_name, "r" );
				if unity_file ~= nil then
					local unity_current_content = unity_file:read("*all");
					if unity_current_content == unity_content then
						create_unity = false;
					end					
					unity_file:close();
				end
			end
			
			if create_unity then
				print( "Create Unity file: " .. path.getbasename( unity_file_name ) .. "." .. ext );
				local unity_file = io.open( unity_file_name, "w" );
				if unity_file ~= nil then
					unity_file:write( unity_content );
					unity_file:close();
				end
			end
			
			files( { unity_file_name } );
		else
			files( all_files );
		end
	end

	self.config:get_config( configuration_obj, platform ):apply( shader_dirs, binary_dirs, binary_files );
end