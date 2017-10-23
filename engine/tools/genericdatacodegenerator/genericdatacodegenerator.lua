-- library/tools/genericdatacodegenerator

local module = Module:new( "genericdatacodegenerator" );

module:add_files( "source/*.*" );
module:add_files( "*.lua" );

module:add_dependency( "config" );
module:add_dependency( "tool/toolbase" );
module:add_dependency( "tool/toolgenericdata" );
--module:add_dependency( "tool/converterbase" );
