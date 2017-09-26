-- library/modules/editor/mechanica_level_renderer

local module = Module:new( "mechanica_level_renderer" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "tool/toolgenericdata" );
module:add_dependency( "editor/entity_template_renderer" );
