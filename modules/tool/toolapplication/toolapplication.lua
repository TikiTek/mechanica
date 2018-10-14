-- library/modules/runtime/toolapplication

local module = Module:new( "toolapplication" );

module:add_files( "include/**/*.*" );
module:add_include_dir( "include" );

module:add_files( "*.lua" );
module:add_files( "source/*.*" );

module:add_dependency( "core/base" );
module:add_dependency( "runtime/framework" );
module:add_dependency( "thirdparty/imgui" );
module:add_dependency( "thirdparty/stb" );
