local module = Module:new();

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_external( "https://github.com/nothings/stb.git" )
module:add_external( "https://sourceforge.net/projects/libpsd" )
