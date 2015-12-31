-- library/modules/core/threading

local module = Module:new( "threading" );

module:add_files( "include/**/*.hpp" );
module:add_files( "threading.lua" );
module:add_include_dir( "include" );

if is_window then
	module:add_files( "source/win/*.*" );
elseif is_linux then
	module:add_files( "source/posix/*.*" );
else
	throw "Platform not supported";
end

