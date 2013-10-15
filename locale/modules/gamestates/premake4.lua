-- locale/modules/gamestates

local module = Module:new( "gamestates" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );
