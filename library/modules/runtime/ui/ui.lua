-- library/modules/runtime/ui

local module = Module:new( "ui" );

module:add_files( "include/tiki/ui/*.*" );
module:add_files( "source/*.*" );
module:add_files( "ui.lua" );
module:add_include_dir( "include" );

module:add_dependency( "base" );
module:add_dependency( "math" );
module:add_dependency( "graphics" );
module:add_dependency( "genericdata" );
module:add_dependency( "uishader" );

local module = Module:new( "uishader" );

module:add_files( "include/tiki/ui/shader/*.hpp" );
module:add_shader_dir( "include" );
