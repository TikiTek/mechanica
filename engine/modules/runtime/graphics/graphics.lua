-- library/modules/runtime/graphics

local module = Module:new( "graphics" );

module:add_files( "source/global/*.*" );
module:add_files( "source/shader/*.fx" );
module:add_files( "include/tiki/graphics/*.*" );
module:add_files( "graphics.lua" );
module:add_include_dir( "include" );

module:add_dependency( "math" );
module:add_dependency( "graphicsbase" );
module:add_dependency( "animation" );
module:add_dependency( "graphicsshader" );

if use_d3d11 then
	module:add_files( "source/win_d3d11/*.*" );
	
	module:add_library_file( "d3d10" );
	module:add_library_file( "d3d11" );
elseif use_d3d12 then
	module:add_files( "source/win_d3d12/*.*" );
	
	module:add_library_file( "d3d12" );
	module:add_library_file( "dxgi" );
	
	module:add_dependency( "d3dx12" );
elseif use_vulkan then
	module:add_files( "source/global_vulkan/*.*" );
	
	module:add_dependency( "vulkan" );
	
	module:add_library_file( "vulkan-1" );
else
	throw( "Graphics API not implemented" );
end

local module = Module:new( "graphicsshader" );

module:add_files( "include/tiki/graphics/shader/*.hpp" );
module:add_shader_dir( "include" );
