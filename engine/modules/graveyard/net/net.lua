-- library/modules/core/net

local module = Module:new( "net" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "net.lua" );
module:add_include_dir( "include" );

module:add_files( "source/win/*.*" );

module:add_dependency( "base" );
module:add_dependency( "container" );
module:add_dependency( "io" );
module:add_dependency( "curl" );
