-- library/tools/converters/fontconverter

local module = Module:new( "fontconverter" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "fontconverter.lua" );
module:add_include_dir( "include" );

module:add_dependency( "runtime/graphicsbase" );
module:add_dependency( "tool/converterbase" );
module:add_dependency( "tool/textureexport" );
module:add_dependency( "thirdparty/freetype" );
