-- library/tools/converters/textureconverter

local module = Module:new( "textureconverter" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "textureconverter.lua" );
module:add_include_dir( "include" );

module:add_dependency( "converterbase" );
module:add_dependency( "textureexport" );
