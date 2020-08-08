local module = Module:new( "asset_converter_interface" );

module:add_files( "include/**/*.hpp" );
module:add_files( "source/*.cpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "libraries/asset_converter_library" );
