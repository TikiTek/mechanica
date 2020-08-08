-- library/modules/tool/asset_converter_library

local module = Module:new( "asset_converter_library" );

module:add_files( "source/*.*" );
module:add_files( "*.lua" );

module:add_dependency( "libraries/asset_converter_interface" );

module:add_dependency( "core/config" );
module:add_dependency( "core/threading" );
module:add_dependency( "tool/tool_project" );

module:add_dependency( "converters/font_converter" );
module:add_dependency( "converters/shader_converter" );
module:add_dependency( "converters/texture_converter" );
module:add_dependency( "converters/model_converter" );
module:add_dependency( "converters/animation_converter" );
module:add_dependency( "converters/generic_data_converter" );
