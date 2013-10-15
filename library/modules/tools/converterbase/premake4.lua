-- library/modules/tools/converterbase

local module = Module:new( "converterbase" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );
