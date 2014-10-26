-- library/thirdparty/directx12

local module = Module:new( "directx12" );

module:add_include_dir( "Include" );

module:add_library_dir( "Lib/x32", nil, "x32" );
module:add_library_dir( "Lib/x64", nil, "x64" );

module:add_binary_dir( "Bin/x32", nil, "x32" );
module:add_binary_dir( "Bin/x64", nil, "x64" );

module:add_binary_file( "d3d12.dll" );
module:add_binary_file( "d3dcompiler_47.dll" );
