
Configuration = class{
	defines = {},
	flags = {},
	binary_dirs = {},
	binary_files = {},
	include_dirs = {},
	library_dirs = {},
	library_files = {},
	shader_dirs = {},
	pre_build_steps = {},
	post_build_steps = {}
};

function Configuration:new()
	return class_instance( self );
end

function Configuration:set_define( name, value )
	if value == nil then
		table.insert( self.defines, name );
	else
		table.insert( self.defines, name .. "=" .. value );
	end
end

function Configuration:checkBasePath( basePath )
	if type( basePath ) ~= "string" then
		throw "[Configuration:checkBasePath] too few arguments.";
	end
end

function Configuration:set_flag( name )
	table.insert( self.flags, name );
end

function Configuration:add_shader_dir( shader_dir, basePath )
	self:checkBasePath( basePath );
	table.insert( self.shader_dirs, path.join( basePath, shader_dir ) );
end

function Configuration:add_binary_dir( binary_dir, basePath )
	self:checkBasePath( basePath );
	table.insert( self.binary_dirs, path.join( basePath, binary_dir ) );
end

function Configuration:add_include_dir( include_dir, basePath )
	self:checkBasePath( basePath );
	table.insert( self.include_dirs, path.join( basePath, include_dir ) );
end

function Configuration:add_library_dir( library_dir, basePath )
	self:checkBasePath( basePath );
	table.insert( self.library_dirs, path.join( basePath, library_dir ) );
end

function Configuration:add_binary_file( binary_filename )
	table.insert( self.binary_files, binary_filename );
end

function Configuration:add_library_file( library_filename )
	table.insert( self.library_files, library_filename );
end

function Configuration:add_pre_build_step( step_script, step_data )
	table.insert( self.pre_build_steps, { script = step_script, data = step_data } );
end

function Configuration:add_post_build_step( step_script, step_data )
	table.insert( self.post_build_steps, { script = step_script, data = step_data } );
end

function Configuration:apply_configuration( target )
	if type( target ) ~= "table" then
		throw "[Configuration:apply_configuration] wrong target arguments.";
	end
		
	target.defines = table.join( target.defines, self.defines );	
	target.flags = table.join( target.flags, self.flags );
	target.include_dirs = table.join( target.include_dirs, self.include_dirs );
	target.library_dirs = table.join( target.library_dirs, self.library_dirs );
	target.library_files = table.join( target.library_files, self.library_files );
	target.binary_dirs = table.join( target.binary_dirs, self.binary_dirs );
	target.binary_files = table.join( target.binary_files, self.binary_files );
	target.shader_dirs = table.join( target.shader_dirs, self.shader_dirs );
	target.pre_build_steps = table.join( target.pre_build_steps, self.pre_build_steps );
	target.post_build_steps = table.join( target.post_build_steps, self.post_build_steps );
end

PlatformConfiguration = class{
	base_path = "",
	globalConfig = nil,
	platforms = {},
	configurations = {},
	platformConfigurations = {} 
};

function PlatformConfiguration:new()
	local platformconfiguration_new = class_instance( self );

	platformconfiguration_new.base_path		= os.getcwd();
	platformconfiguration_new.globalConfig	= Configuration:new();

	return platformconfiguration_new;
end

function PlatformConfiguration:get_config( configuration, platform )
	if ( ( configuration ~= nil and type( configuration ) ~= "string" ) or ( platform ~= nil and type( platform ) ~= "string" ) ) then
		throw "[PlatformConfiguration:get_config] Invalid args";
	end

	if ( configuration ~= nil and platform ~= nil ) then
		if not self.platformConfigurations[ platform ] then
			self.platformConfigurations[ platform ] = { configurations = {} };
		end
		if not self.platformConfigurations[ platform ].configurations[ configuration ] then
			self.platformConfigurations[ platform ].configurations[ configuration ] = Configuration:new();
		end

		return self.platformConfigurations[ platform ].configurations[ configuration ];
	elseif ( configuration ~= nil and platform == nil ) then
		if not self.configurations[ configuration ] then
			self.configurations[ configuration ] = Configuration:new();
		end

		return self.configurations[ configuration ];
	elseif ( configuration == nil and platform ~= nil ) then
		if not self.platforms[ platform ] then
			self.platforms[ platform ] = Configuration:new();
		end

		return self.platforms[ platform ];
	else
		return self.globalConfig;
	end

	return nil;
end

function PlatformConfiguration:set_base_path( base_path )
	self.base_path = path.join( global_configuration.root_path, base_path );
end

function PlatformConfiguration:set_define( name, value, configuration, platform )
	if ( type( name ) == "string" and ( value == nil or type( value ) == "string" ) ) then
		self:get_config( configuration, platform ):set_define( name, value, self.base_path );
	else
		throw("[set_define] Invalid args.")
	end
end

function PlatformConfiguration:set_flag( name, configuration, platform )
	if type( name ) == "string" then
		self:get_config( configuration, platform ):set_flag( name, self.base_path );
	else
		throw("[set_flag] Invalid args.")
	end
end

function PlatformConfiguration:add_shader_dir( shader_dir, configuration, platform )
	if type( shader_dir ) == "string" then
		self:get_config( configuration, platform ):add_shader_dir( shader_dir, self.base_path );
	else
		throw "[add_shader_dir] Invalid args.";
	end
end

function PlatformConfiguration:add_binary_dir( binary_dir, configuration, platform )
	if type( binary_dir ) == "string" then
		self:get_config( configuration, platform ):add_binary_dir( binary_dir, self.base_path );
	else
		throw "[add_binary_dir] Invalid args.";
	end
end

function PlatformConfiguration:add_include_dir( include_dir, configuration, platform )
	if type( include_dir ) == "string" then
		self:get_config( configuration, platform ):add_include_dir( include_dir, self.base_path );
	else
		throw "[add_binary_dir] Invalid args.";
	end
end

function PlatformConfiguration:add_library_dir( library_dir, configuration, platform )
	if type( library_dir ) == "string" then
		self:get_config( configuration, platform ):add_library_dir( library_dir, self.base_path );
	else
		throw "[add_library_dir] Invalid args.";
	end
end

function PlatformConfiguration:add_binary_file( binary_filename, configuration, platform )
	if type( binary_filename ) == "string" then
		self:get_config( configuration, platform ):add_binary_file( binary_filename );
	else
		throw "[add_binary_file] Invalid args.";
	end
end

function PlatformConfiguration:add_library_file( library_filename, configuration, platform )
	if type( library_filename ) == "string" then
		self:get_config( configuration, platform ):add_library_file( library_filename );
	else
		throw "[add_library_file] Invalid args.";
	end
end

function PlatformConfiguration:add_pre_build_step( step_script, step_data, configuration, platform )
	if type( step_script ) == "string" and type( step_data ) == "table" then
		self:get_config( configuration, platform ):add_pre_build_step( step_script, step_data );
	else
		throw "[add_pre_build_step] Invalid args.";
	end
end

function PlatformConfiguration:add_post_build_step( step_script, step_data, configuration, platform )
	if type( step_script ) == "string" and type( step_data ) == "table" then
		self:get_config( configuration, platform ):add_post_build_step( step_script, step_data );
	else
		throw "[add_post_build_step] Invalid args.";
	end
end
