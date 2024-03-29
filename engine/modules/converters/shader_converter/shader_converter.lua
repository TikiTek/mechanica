local module = Module:new();

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "runtime/graphics_base" );
module:add_dependency( "tool/tool_base" );
module:add_dependency( "tool/converter_base" );

if tiki.target_platform == Platforms.Windows then
	module:add_library_file( "d3dcompiler" );
	--module:add_binary_file( "d3dcompiler_46.dll" );
end
