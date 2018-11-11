-- library/modules/runtime/ui

local module = Module:new( "uishader" );

module:add_files( "include/tiki/ui/shader/*.hpp" );
module:add_shader_dir( "include" );

local module = Module:new( "ui" );

module:add_files( "include/tiki/ui/*.*" );
module:add_files( "source/shader/*.fx" );
module:add_files( "source/*.*" );
module:add_files( "ui.lua" );
module:add_include_dir( "include" );

module:add_dependency( "core/base" );
module:add_dependency( "core/math" );
module:add_dependency( "core/genericdata" );
module:add_dependency( "runtime/graphics" );
module:add_dependency( "runtime/script" );
module:add_dependency( "runtime/uishader" );
