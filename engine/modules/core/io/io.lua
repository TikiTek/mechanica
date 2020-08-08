local module = Module:new();

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

if tiki.platform == Platforms.Windows then
	module:add_files( "source/win/*.*" );
elseif tiki.platform == Platforms.Linux then
	module:add_files( "source/posix/*.*" );
else
	throw "Platform not supported";
end

module:add_dependency( "core/base" );
module:add_dependency( "core/threading" );
module:add_dependency( "third_party/dbalsterxml" );
