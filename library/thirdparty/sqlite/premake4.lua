-- library/thirdparty/sqlite

local module = Module:new( "sqlite" );

module:add_files( "source/*.c" );
module:add_files( "include/**/*.h" );
module:add_include_dir( "include" );
