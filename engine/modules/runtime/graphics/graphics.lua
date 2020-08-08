GraphicsApis = {
	Stub,
	D3D11 = 0,
	D3D12 = 1,
	Vulkan = 2
}

local api = GraphicsApis.Stub
if tiki.platform == Platforms.Windows then
	api = GraphicsApis.D3D11
elseif tiki.platform == Platforms.Linux then
	api = GraphicsApis.Vulkan
else
	throw( "Operating System not supported." );
end

local module = Module:new( "graphics_shader" );

module:add_files( "include/tiki/graphics/shader/*.hpp" );

local module = Module:new( "graphics" );

module:set_define( "TIKI_GRAPHICS_D3D11", iff( api == GraphicsApis.D3D11, "TIKI_ON", "TIKI_OFF" ) );
module:set_define( "TIKI_GRAPHICS_D3D12", iff( api == GraphicsApis.D3D12, "TIKI_ON", "TIKI_OFF" ) );
module:set_define( "TIKI_GRAPHICS_VULKAN", iff( api == GraphicsApis.Vulkan, "TIKI_ON", "TIKI_OFF" ) );

module:add_files( "source/global/*.*" );
module:add_files( "source/shader/*.fx" );
module:add_files( "include/tiki/graphics/*.*" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "core/math" );
module:add_dependency( "runtime/graphics_base" );
module:add_dependency( "runtime/animation" );

module:add_dependency( "graphics_shader" );

if api == GraphicsApis.D3D11 then
	module:add_files( "source/win_d3d11/*.*" );
	
	module:add_library_file( "d3d10" );
	module:add_library_file( "d3d11" );
elseif api == GraphicsApis.D3D12 then
	module:add_files( "source/win_d3d12/*.*" );
	
	module:add_library_file( "d3d12" );
	module:add_library_file( "dxgi" );
	
	module:add_dependency( "d3dx12" );
--elseif api == GraphicsApis.Vulkan then
	--module:add_files( "source/global_bgfx/*.*" );
	
	--module:add_dependency( "third_party/bfgx" );
else
	throw( "Graphics API not implemented" );
end
