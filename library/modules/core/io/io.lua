-- library/modules/core/io

local module = Module:new( "io" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "io.lua" );
module:add_include_dir( "include" );

if is_window then
	module:add_files( "source/win/*.*" );
elseif is_linux then
	module:add_files( "source/posix/*.*" );
else
	throw "Platform not supported";
end

module:add_dependency( "base" );
module:add_dependency( "threading" );
module:add_dependency( "dbalsterxml" );
