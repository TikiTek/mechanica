-- library/tools/converters/animationconverter

local module = Module:new( "animationconverter" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "animationconverter.lua" );
module:add_include_dir( "include" );

module:add_dependency( "converterbase" );
module:add_dependency( "math" );
module:add_dependency( "modelexport" );
