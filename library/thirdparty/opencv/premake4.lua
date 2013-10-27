-- library/thirdparty/opencv

local module = Module:new( "opencv" );

module:add_binary_dir( "bin/x32" );
module:add_library_dir( "lib/x32" );

module:add_files( "include/**/*.h" );
module:add_include_dir( "include" );
