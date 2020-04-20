local module = Module:new();

module:add_files( "source/*.*" );
module:add_files( "*.lua" );

module:add_dependency( "config" );
module:add_dependency( "core/threading" );
module:add_dependency( "libraries/asset_converter_interface" );
