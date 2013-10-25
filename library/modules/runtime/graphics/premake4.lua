-- library/modules/runtime/graphics

local module = Module:new( "graphics" );

module:add_files( "source/global/*.*" );
module:add_files( "include/tiki/graphics/*.hpp" );
module:add_include_dir( "include" );

module:add_dependency( "math" );
module:add_dependency( "graphicsbase" );
--module:add_dependency( "graphicscomponents" );
module:add_dependency( "graphicsresources" );

if is_windows then
	module:set_define( "TIKI_GRAPHICS_D3D11", "TIKI_ON" );
	module:set_define( "TIKI_GRAPHICS_OPENGL4", "TIKI_OFF" );
elseif is_linux then
	module:set_define( "TIKI_GRAPHICS_D3D11", "TIKI_OFF" );
	module:set_define( "TIKI_GRAPHICS_OPENGL4", "TIKI_ON" );
else
	throw "Platform not implemented"
end