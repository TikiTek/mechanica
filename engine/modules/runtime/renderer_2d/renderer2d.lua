-- library/modules/runtime/renderer2d

local module = Module:new( "renderer2dshader" );

module:add_files( "source/shader/*.*" );
module:add_shader_dir( "source" );

module:add_dependency( "graphicsshader" );

local module = Module:new( "renderer2d" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "runtime/graphics" );
module:add_dependency( "runtime/graphicsresources" );
module:add_dependency( "runtime/renderer2dshader" );
module:add_dependency( "runtime/rendereffects" );

