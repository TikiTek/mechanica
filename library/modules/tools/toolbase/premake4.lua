-- library/modules/tools/toolbase

local module = Module:new( "toolbase" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );
