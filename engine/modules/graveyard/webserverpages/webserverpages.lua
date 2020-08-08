local module = Module:new( "webserverpages" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "webserverpages.lua" );
module:add_include_dir( "include" );

module:add_dependency( "base" );
module:add_dependency( "toollibraries" );
