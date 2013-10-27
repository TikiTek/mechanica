-- library/tools/converters/materialconverter

local module = Module:new( "materialconverter" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );

module:add_dependency( "converterbase" );
