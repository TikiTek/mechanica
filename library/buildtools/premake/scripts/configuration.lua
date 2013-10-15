
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
		defines( { k.."="..v } );
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
	if ( configuration and platform ) then
		if not self.platforms[ platform ] then
			self.platforms[ platform ] = { configurations = {} };
		end
		if not self.platforms[ platform ].configurations[ configuration ] then
			self.platforms[ platform ].configurations[ configuration ] = Configuration:new();
		end

		return self.platforms[ platform ].configurations[ configuration ];
	elseif ( configuration ) then
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
	if ( name and value ) then
		self:get_config( configuration, platform ):set_define( name, value );
	else
		error("Not enough arguments.")
	end
end

function PlatformConfiguration:set_flag( name, configuration, platform )
	if ( name ) then
		self:get_config( configuration, platform ):set_flag( name );
	else
		error("Not enough arguments.")
	end
end