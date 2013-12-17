-- locale/modules/t3renderer

local module = Module:new( "t3renderer" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );
