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

if is_windows then
	module:set_define( "TIKI_GRAPHICS_D3D11", "TIKI_ON" );
	module:set_define( "TIKI_GRAPHICS_OPENGL4", "TIKI_OFF" );

	module:add_files( "source/win_d3d11/*.*" );
elseif is_linux then
	module:set_define( "TIKI_GRAPHICS_D3D11", "TIKI_OFF" );
	module:set_define( "TIKI_GRAPHICS_OPENGL4", "TIKI_ON" );

	module:add_files( "source/global_opengl4/*.*" );
else
	throw "Platform not implemented"
end