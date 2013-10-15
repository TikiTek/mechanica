-- library/modules/tools/textureexport

local module = Module:new( "textureexport" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );
