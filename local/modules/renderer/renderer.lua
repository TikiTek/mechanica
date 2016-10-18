-- local/modules/renderer

local module = Module:new( "renderer" );

module:add_files( "source/*.*" );
module:add_files( "source/**/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "graphics" );
module:add_dependency( "graphicsresources" );
module:add_dependency( "renderershader" );

local module = Module:new( "renderershader" );

module:add_files( "source/shader/*.*" );
module:add_shader_dir( "source" );

module:add_dependency( "graphicsshader" );
module:add_dependency( "uishader" );
