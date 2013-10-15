-- library/modules/tools/modelexport

local module = Module:new( "modelexport" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );
