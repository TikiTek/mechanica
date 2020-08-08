-- library/tests/nettest

local module = Module:new( "nettest" );

module:add_files( "source/*.*" );
module:add_files( "nettest.lua" );

module:add_dependency( "config" );
module:add_dependency( "core/base" );
module:add_dependency( "runtime/network" );
