
Configuration = class{ defines = {}, flags = {}, shader_dirs = {}, binary_dirs = {}, include_dirs = {}, library_dirs = {}, binary_files = {}, library_files = {} };

function Configuration:new()
	local configuration_new = class_instance( self );

	return configuration_new;
end

function Configuration:set_define( name, value )
	self.defines[ name ] = value;
end

function Configuration:set_flag( name )
	table.insert( self.flags, name );
end

function Configuration:add_shader_dir( shader_dir )
	table.insert( self.shader_dirs, path.getabsolute( shader_dir ) );
end

function Configuration:add_binary_dir( binary_dir )
	table.insert( self.binary_dirs, path.getabsolute( binary_dir ) );
end

function Configuration:add_include_dir( include_dir )
	table.insert( self.include_dirs, path.getabsolute( include_dir ) );
end

function Configuration:add_library_dir( library_dir )
	table.insert( self.library_dirs, path.getabsolute( library_dir ) );
end

function Configuration:add_binary_file( binary_filename )
	table.insert( self.binary_files, binary_filename );
end

function Configuration:add_library_file( library_filename )
	table.insert( self.library_files, library_filename );
end

function Configuration:apply( shader_dirs, binary_dirs, binary_files )
	if ( binary_dirs == nil or  binary_files == nil ) then
		throw "[Configuration:apply] too few arguments.";
	end

	for k, v in pairs( self.defines ) do
		if v ~= nil then
			defines( { k.."="..v } );
			--print( k.."="..v );
		else
			defines( { k } );
			--print( k );
		end
	end

	flags( self.flags );

	includedirs( self.include_dirs );
	libdirs( self.library_dirs );		
	links( self.library_files );

	if shader_dirs ~= nil then
		for i,dir in pairs( self.shader_dirs ) do
			table.insert( shader_dirs, dir );
		end
	end

	for i,dir in pairs( self.binary_dirs ) do
		table.insert( binary_dirs, dir );
	end

	for i,file in pairs( self.binary_files ) do
		table.insert( binary_files, file );
	end

	--[[for i,dir in pairs( self.library_dirs ) do
		print( "libdir: " .. dir );
	end

	for i,file in pairs( self.library_files ) do
		print( "libfile: " .. file );
	end]]--
end

PlatformConfiguration = class{ global_config = nil, platforms = {}, configurations = {}, platformconfigurations = {} };

function PlatformConfiguration:new()
	local platformconfiguration_new = class_instance( self );

	platformconfiguration_new.global_config = Configuration:new();

	return platformconfiguration_new;
end

function PlatformConfiguration:get_config( configuration, platform )
	if ( ( configuration ~= nil and type( configuration ) ~= "string" ) or ( platform ~= nil and type( platform ) ~= "string" ) ) then
		throw "[PlatformConfiguration:get_config] Invalid args";
	end

	if ( configuration ~= nil and platform ~= nil ) then
		if not self.platformconfigurations[ platform ] then
			self.platformconfigurations[ platform ] = { configurations = {} };
		end
		if not self.platformconfigurations[ platform ].configurations[ configuration ] then
			self.platformconfigurations[ platform ].configurations[ configuration ] = Configuration:new();
		end

		return self.platformconfigurations[ platform ].configurations[ configuration ];
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
		return self.global_config;
	end

	return nil;
end

function PlatformConfiguration:set_define( name, value, configuration, platform )
	if ( type( name ) == "string" and ( value == nil or type( value ) == "string" ) ) then
		self:get_config( configuration, platform ):set_define( name, value );
	else
		throw("[set_define] Invalid args.")
	end
end

function PlatformConfiguration:set_flag( name, configuration, platform )
	if type( name ) == "string" then
		self:get_config( configuration, platform ):set_flag( name );
	else
		throw("[set_flag] Invalid args.")
	end
end

function PlatformConfiguration:add_shader_dir( shader_dir, configuration, platform )
	if type( shader_dir ) == "string" then
		self:get_config( configuration, platform ):add_shader_dir( shader_dir );
	else
		throw "[add_shader_dir] Invalid args.";
	end
end

function PlatformConfiguration:add_binary_dir( binary_dir, configuration, platform )
	if type( binary_dir ) == "string" then
		self:get_config( configuration, platform ):add_binary_dir( binary_dir );
	else
		throw "[add_binary_dir] Invalid args.";
	end
end

function PlatformConfiguration:add_include_dir( include_dir, configuration, platform )
	if type( include_dir ) == "string" then
		self:get_config( configuration, platform ):add_include_dir( include_dir );
	else
		throw "[add_binary_dir] Invalid args.";
	end
end

function PlatformConfiguration:add_library_dir( library_dir, configuration, platform )
	if type( library_dir ) == "string" then
		self:get_config( configuration, platform ):add_library_dir( library_dir );
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