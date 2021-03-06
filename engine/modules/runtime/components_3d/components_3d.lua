local module = Module:new( "components3d" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "runtime/componentbase" );
module:add_dependency( "runtime/entitysystem" );
