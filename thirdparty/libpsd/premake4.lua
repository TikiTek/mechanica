-- library/thirdparty/libpsd

local module = Module:new( "libpsd" );

module:add_files( "src/*.c" );
module:add_files( "include/*.h" );
module:add_include_dir( "include" );
