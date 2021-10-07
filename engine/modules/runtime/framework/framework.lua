local module = Module:new();

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

--if use_sdl then
--	module:add_files( "source/sdl/*.*" );
--	module:add_dependency( "sdl" );
if tiki.target_platform == Platforms.Windows then
	module:add_files( "source/win/*.*" );
else
	throw "Platform not implemented"
end

module:add_dependency( "core/resource" );
module:add_dependency( "runtime/graphics" );
module:add_dependency( "runtime/input" );
module:add_dependency( "runtime/runtime_shared" );
module:add_dependency( "runtime/debug_renderer" );
module:add_dependency( "tool/tool_xml" );
