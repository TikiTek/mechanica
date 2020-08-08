local module = Module:new( "animation" );

module:add_files( "include/**/*.hpp" );
module:add_files( "source/*.*" );
module:add_files( "animation.lua" );
module:add_include_dir( "include" );

module:add_dependency( "base" );
module:add_dependency( "math" );
