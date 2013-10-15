
Platform = class{ name = nil, configurations = {} };

function Platform:new( name, configs )
	local platform_new = class_instance( self );

	platform_new.name = name;
	
	for i,configuration in pairs( configs ) do
		platform_new.configurations[ configuration ] = Configuration:new( configuration );
	end

	return platform_new;
end

function Platform:get_name()
	return self.name;
end

function Platform:get_config( name )
	return self.configurations[ name ];
end
