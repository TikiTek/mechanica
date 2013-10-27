-- library/thirdparty/trex

local module = Module:new( "trex" );

module:add_files( "source/*.c" );
module:add_files( "include/*.h" );
module:add_include_dir( "include" );
