local module = Module:new();

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "core/math" );
module:add_dependency( "tool/converter_base" );
module:add_dependency( "tool/model_export" );
