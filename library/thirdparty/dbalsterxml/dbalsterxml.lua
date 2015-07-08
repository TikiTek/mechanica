-- library/thirdparty/dbalsterxml

local module = Module:new( "dbalsterxml" );

module:add_files( "source/*.*" );
module:add_files( "include/*.h" );
module:add_include_dir( "include" );
