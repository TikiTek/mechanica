-- library/tools/converters/genericdataconverter

local module = Module:new( "genericdataconverter" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );

module:add_dependency( "converterbase" );
module:add_dependency( "toolgenericdata" );
