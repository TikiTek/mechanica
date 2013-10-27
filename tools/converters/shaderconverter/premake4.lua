-- library/tools/converters/shaderconverter

local module = Module:new( "shaderconverter" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );

module:add_dependency( "converterbase" );
module:add_dependency( "graphicsbase" );
module:add_dependency( "trex" );

module:add_library( "d3dcompiler" );
