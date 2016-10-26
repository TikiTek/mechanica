-- library/thirdparty/directx

local module = Module:new( "directx" );

module:add_files( "*.lua" );

module:add_include_dir( "$(DXSDK_DIR)/Include" );

module:add_library_dir( "$(DXSDK_DIR)/Lib/x86", nil, "x32" );
module:add_library_dir( "$(DXSDK_DIR)/Lib/x64", nil, "x64" );
