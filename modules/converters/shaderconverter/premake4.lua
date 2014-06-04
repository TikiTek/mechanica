-- library/tools/converters/shaderconverter

local module = Module:new( "shaderconverter" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );

module:add_dependency( "converterbase" );
module:add_dependency( "graphicsbase" );
module:add_dependency( "trex" );
module:add_dependency( "gpp" );
module:add_dependency( "dlls" );

module:add_library_file( "d3dcompiler" );

module:add_binary_file( "d3dcompiler_46.dll" );
