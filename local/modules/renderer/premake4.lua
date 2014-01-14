-- locale/modules/renderer

local module = Module:new( "renderer" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );

module:add_dependency( "graphics" );
module:add_dependency( "rendereffectdata" );
