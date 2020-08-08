local module = Module:new( "graphics_shader" );

module:add_files( "include/tiki/graphics/shader/*.hpp" );

local module = Module:new( "graphics" );

module:add_files( "source/global/*.*" );
module:add_files( "source/shader/*.fx" );
module:add_files( "include/tiki/graphics/*.*" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "core/math" );
module:add_dependency( "runtime/graphics_base" );
module:add_dependency( "runtime/animation" );

module:add_dependency( "graphics_shader" );

if use_d3d11 then
	module:add_files( "source/win_d3d11/*.*" );
	
	module:add_library_file( "d3d10" );
	module:add_library_file( "d3d11" );
elseif use_d3d12 then
	module:add_files( "source/win_d3d12/*.*" );
	
	module:add_library_file( "d3d12" );
	module:add_library_file( "dxgi" );
	
	module:add_dependency( "d3dx12" );
elseif use_bgfx then
	module:add_files( "source/global_bgfx/*.*" );
	
	module:add_dependency( "third_party/bfgx" );
else
	throw( "Graphics API not implemented" );
end
