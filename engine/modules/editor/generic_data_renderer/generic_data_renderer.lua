-- library/modules/editor/generic_data_renderer

local module = Module:new( "generic_data_renderer" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "tool/toolgenericdata" );
module:add_dependency( "editor/entity_template_renderer" );
