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

use_d3d11 = false; --is_windows;
use_opengl = true; --is_linux;

if use_d3d11 then
	module:set_define( "TIKI_GRAPHICS_D3D11", "TIKI_ON" );
	module:set_define( "TIKI_GRAPHICS_OPENGL4", "TIKI_OFF" );

	module:add_files( "source/win_d3d11/*.*" );
	
	module:add_library_file( "d3d10" );
	module:add_library_file( "d3d11" );
elseif use_opengl then
	module:set_define( "TIKI_GRAPHICS_D3D11", "TIKI_OFF" );
	module:set_define( "TIKI_GRAPHICS_OPENGL4", "TIKI_ON" );

	module:add_files( "source/global_opengl4/*.*" );
	
	module:add_dependency( "glew" );
	
	module:add_library_file( "opengl32" );
else
	throw( "Graphics API not implemented" );
end