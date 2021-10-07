local module = Module:new( "base" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "*.lua" );
module:add_files( "*.natvis" );
module:add_include_dir( "include" );

if tiki.target_platform == Platforms.Windows then
	module:add_files( "source/win/*.cpp" );
elseif tiki.target_platform == Platforms.Linux then
	module:add_files( "source/posix/*.cpp" );

	module:add_library_file( "pthread" );
else
	throw("Platform not supported.");
end

module:add_dependency( "core/io" );
