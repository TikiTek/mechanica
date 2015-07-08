-- library/tools/converters/fontconverter

local module = Module:new( "fontconverter" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );

module:add_dependency( "converterbase" );
module:add_dependency( "graphicsbase" );
module:add_dependency( "freetype" );
module:add_dependency( "textureexport" );
