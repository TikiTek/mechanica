
Configuration = class{ defines = {}, flags = {} };

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

function Configuration:apply()
	for k, v in pairs( self.defines ) do
		if v ~= nil then
			defines( { k.."="..v } );
			print( k.."="..v );
		else
			defines( { k } );
			print( k );
		end
	end

	flags( self.flags );
end

PlatformConfiguration = class{ global_config = nil, platforms = {}, configurations = {} };

function PlatformConfiguration:new()
	local platformconfiguration_new = class_instance( self );

	platformconfiguration_new.global_config = Configuration:new();

	return platformconfiguration_new;
end

function PlatformConfiguration:get_config( configuration, platform )
	if ( ( configuration ~= nil and type( configuration ) ~= "string" ) or ( platform ~= nil and type( platform ) ~= "string" ) ) then
		throw( "[get_config] Invalid args" );
	end

	if ( configuration and platform ) then
		if not self.platforms[ platform ] then
			self.platforms[ platform ] = { configurations = {} };
		end
		if not self.platforms[ platform ].configurations[ configuration ] then
			self.platforms[ platform ].configurations[ configuration ] = Configuration:new();
		end

		return self.platforms[ platform ].configurations[ configuration ];
	elseif configuration then
		if not self.configurations[ configuration ] then
			self.configurations[ configuration ] = Configuration:new();
		end

		return self.configurations[ configuration ];
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