-- library/modules/core/io

local module = Module:new( "io" );

module:add_files( "source/*.*" );
module:add_files( "source/win/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "io.lua" );
module:add_include_dir( "include" );

module:add_dependency( "base" );
module:add_dependency( "threading" );
module:add_dependency( "dbalsterxml" );
