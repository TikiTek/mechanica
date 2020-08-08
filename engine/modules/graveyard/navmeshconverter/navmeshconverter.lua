local module = Module:new( "navmeshconverter" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "navmeshconverter.lua" );
module:add_include_dir( "include" );

module:add_dependency( "converterbase" );
