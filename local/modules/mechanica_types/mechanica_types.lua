-- local/modules/mechanica_types

local module = Module:new( "mechanica_types" );

--module:add_files( "source/*.*" );
--module:add_files( "include/**/*.hpp" );
module:add_files( path.join( global_configuration.root_path, "content/mechanica/genericdata/types/*.*" ) );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "base" );
module:add_dependency( "genericdata" );

