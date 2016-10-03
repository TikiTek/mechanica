-- config

local module = Module:new( "config" );

module.import_func = function()
	if is_windows then
		use_d3d11	= true;
		--use_d3d12	= true;
		--use_vulkan	= true;
	elseif is_linux then
		use_vulkan	= true;
	elseif is_apple then
		use_vulkan	= true;
	else
		throw("Operating System not supported.");
	end
end

module:add_dependency( "libraryconfig" );

module:set_flag( "EnableMinimalRebuild" );
