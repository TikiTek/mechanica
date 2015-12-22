-- library/modules/runtime/gameapplication

local module = Module:new( "gameapplication" );

module:add_files( "include/*.*" );
module:add_include_dir( "include" );
module:add_files( "source/*.*" );

if use_sdl then
	module:add_files( "source/sdl/*.*" );
	module:add_dependency( "sdl" );
elseif is_windows then
	module:add_files( "source/win/*.*" );
else
	throw "Platform not implemented"
end

module:add_dependency( "base" );
module:add_dependency( "framework" );
