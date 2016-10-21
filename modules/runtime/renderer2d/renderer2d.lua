-- library/modules/runtime/renderer2d

local module = Module:new( "renderer2d" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "graphics" );
module:add_dependency( "graphicsresources" );
module:add_dependency( "renderer2dshader" );
module:add_dependency( "rendereffects" );

local module = Module:new( "renderer2dshader" );

module:add_files( "source/shader/*.*" );
module:add_shader_dir( "source" );

module:add_dependency( "graphicsshader" );
