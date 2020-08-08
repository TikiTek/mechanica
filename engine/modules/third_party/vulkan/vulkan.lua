local module = Module:new( "vulkan" );

module:add_files( "vulkan.lua" );

local vulkan_sdk = os.getenv( "VULKAN_SDK" ) or "";
if not os.isdir( vulkan_sdk ) then
	throw("Vulkan SDK not found: " .. vulkan_sdk );
end
module:set_base_path( vulkan_sdk );

module:add_library_dir( "Source/lib32", nil, "x32" );
module:add_library_dir( "Source/lib", nil, "x64" );

module:add_include_dir( "Include" );
