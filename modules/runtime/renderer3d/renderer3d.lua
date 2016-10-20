-- library/modules/runtime/renderer3d

local module = Module:new( "renderer3d" );

module:add_files( "source/*.*" );
module:add_files( "source/**/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "graphics" );
module:add_dependency( "graphicsresources" );
module:add_dependency( "renderer3dshader" );

local module = Module:new( "renderer3dshader" );

module:add_files( "source/shader/*.*" );
module:add_shader_dir( "source" );

module:add_dependency( "graphicsshader" );
module:add_dependency( "uishader" );
