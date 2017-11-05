-- library/modules/tool/asset_converter_library

local module = Module:new( "asset_converter_library" );

module:add_files( "source/*.*" );
module:add_files( "*.lua" );

module:add_dependency( "libraries/asset_converter_interface" );

module:add_dependency( "core/config" );
module:add_dependency( "core/threading" );
module:add_dependency( "tool/toolproject" );

module:add_dependency( "converters/fontconverter" );
module:add_dependency( "converters/shaderconverter" );
module:add_dependency( "converters/textureconverter" );
module:add_dependency( "converters/modelconverter" );
module:add_dependency( "converters/animationconverter" );
module:add_dependency( "converters/genericdataconverter" );
