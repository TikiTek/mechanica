local module = Module:new( "tool_application" );

module:add_files( "include/**/*.*" );
module:add_include_dir( "include" );

module:add_files( "*.lua" );
module:add_files( "source/*.*" );

module:add_dependency( "core/base" );
module:add_dependency( "runtime/framework" );
module:add_dependency( "third_party/imgui" );
module:add_dependency( "third_party/stb" );

module:add_resources( "resource/*.png" );
