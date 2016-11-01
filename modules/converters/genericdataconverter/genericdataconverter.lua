-- library/tools/converters/genericdataconverter

local module = Module:new( "genericdataconverter" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "tool/converterbase" );
module:add_dependency( "tool/toolgenericdata" );

module:set_define( "TIKI_GENERICDATA_CONVERTER", "TIKI_ON" );