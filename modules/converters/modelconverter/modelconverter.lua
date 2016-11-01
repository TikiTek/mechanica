-- library/tools/converters/modelconverter

local module = Module:new( "modelconverter" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "modelconverter.lua" );
module:add_include_dir( "include" );

module:add_dependency( "tool/converterbase" );
module:add_dependency( "tool/modelexport" );
