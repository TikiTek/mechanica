-- library/modules/tool/textureexport

local module = Module:new( "textureexport" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "textureexport.lua" );
module:add_include_dir( "include" );

module:add_dependency( "libpsd" );
