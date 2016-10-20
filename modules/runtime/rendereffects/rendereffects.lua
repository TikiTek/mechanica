-- library/modules/runtime/rendereffects

local module = Module:new( "rendereffects" );

module:add_files( "source/*.*" );
module:add_files( "source/**/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "graphics" );
module:add_dependency( "graphicsresources" );
module:add_dependency( "rendereffectshader" );

local module = Module:new( "rendereffectshader" );

module:add_files( "source/shader/*.*" );
module:add_shader_dir( "source" );

module:add_dependency( "graphicsshader" );
