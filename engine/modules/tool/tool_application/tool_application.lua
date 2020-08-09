local module = Module:new( "tool_application" )

module:add_files( "include/**/*.*" )
module:add_include_dir( "include" )

module:add_files( "*.lua" )
module:add_files( "source/*.*" )

module:add_dependency( "core/base" )
module:add_dependency( "runtime/framework" )

module:add_external( "https://github.com/ocornut/imgui.git" )
module:add_external( "https://github.com/nothings/stb.git" )

module:add_resources( "resource/*.png" )
