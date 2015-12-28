-- library/thirdparty/dlls

local module = Module:new( "dlls" );

module:add_files( "dlls.lua" );

module:add_binary_dir( "bin/x32", nil, "x32" );
module:add_binary_dir( "bin/x64", nil, "x64" );
