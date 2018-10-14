-- library/modules/runtime/framework

local module = Module:new( "framework" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

if use_sdl then
	module:add_files( "source/sdl/*.*" );
	module:add_dependency( "sdl" );
elseif is_windows then
	module:add_files( "source/win/*.*" );
else
	throw "Platform not implemented"
end

module:add_dependency( "core/resource" );
module:add_dependency( "runtime/graphics" );
module:add_dependency( "runtime/input" );
module:add_dependency( "runtime/debuggui" );
module:add_dependency( "runtime/debugguiwindows" );
module:add_dependency( "runtime/runtimeshared" );
module:add_dependency( "runtime/debugrenderer" );
