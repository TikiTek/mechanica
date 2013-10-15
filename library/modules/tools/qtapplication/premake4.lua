-- library/modules/tools/qtapplication

local module = Module:new( "qtapplication" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );
