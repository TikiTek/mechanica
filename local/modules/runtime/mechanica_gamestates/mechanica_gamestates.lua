-- local/modules/mechanica_gamestates

local module = Module:new( "mechanica_gamestates" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "runtime/framework" );
module:add_dependency( "runtime/graphics" );
module:add_dependency( "runtime/resource" );
module:add_dependency( "runtime/renderer2d" );
module:add_dependency( "runtime/gameflowsystem" );

module:add_dependency( "runtime/mechanica_gameplay" );
