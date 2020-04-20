local module = Module:new( "render_effect_shader" );

module:add_files( "source/shader/*.*" );

module:add_dependency( "graphics_shader" );

local module = Module:new( "render_effects" );

module:add_files( "source/*.*" );
module:add_files( "source/**/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "runtime/graphics" );
module:add_dependency( "runtime/graphics_resources" );
module:add_dependency( "runtime/render_effect_shader" );
