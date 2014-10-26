-- library/modules/runtime/graphics

local module = Module:new( "graphics" );

module:add_files( "source/global/*.*" );
module:add_files( "source/shader/*.fx" );
module:add_files( "include/**/*.*" );
module:add_include_dir( "include" );
module:add_shader_dir( "include" );

module:add_dependency( "math" );
module:add_dependency( "graphicsbase" );
module:add_dependency( "animation" );

if use_d3d11 then
	module:add_files( "source/win_d3d11/*.*" );
	
	module:add_library_file( "d3d10" );
	module:add_library_file( "d3d11" );
elseif use_d3d12 then
	module:add_files( "source/win_d3d12/*.*" );
	
	module:add_dependency( "directx12" );
	module:add_library_file( "d3d12" );
elseif use_opengl then
	module:add_files( "source/global_opengl4/*.*" );
	
	module:add_dependency( "glew" );
	
	module:add_library_file( "opengl32" );
	module:add_library_file( "glu32" );
else
	throw( "Graphics API not implemented" );
end