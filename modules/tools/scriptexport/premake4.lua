-- library/modules/tools/scriptexport

local module = Module:new( "scriptexport" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );
