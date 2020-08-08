local module = Module:new( "renderer_2d_shader" );

module:add_files( "source/shader/*.*" );

module:add_dependency( "graphics_shader" );

local module = Module:new( "renderer_2d" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "runtime/graphics" );
module:add_dependency( "runtime/graphics_resources" );
module:add_dependency( "runtime/renderer_2d_shader" );
module:add_dependency( "runtime/render_effects" );

