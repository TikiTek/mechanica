local module = Module:new( "dbalsterxml" );

module:add_files( "include/*.h" );
module:add_files( "source/*.*" );
module:add_files( "dbalsterxml.lua" );
module:add_include_dir( "include" );
