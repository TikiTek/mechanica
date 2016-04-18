-- library/thirdparty/vulkan

local module = Module:new( "vulkan" );

module:add_files( "vulkan.lua" );

if not os.isdir( os.getenv( "VULKAN_SDK" ) ) then
	throw("Vulkan SDK not found: " .. os.getenv( "VULKAN_SDK" ) );
end
module:set_base_path( os.getenv("VULKAN_SDK") );

module:add_library_dir( "Source/lib32", nil, "x32" );
module:add_library_dir( "Source/lib", nil, "x64" );

module:add_include_dir( "Include" );
