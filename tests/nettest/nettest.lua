-- library/tests/nettest

local module = Module:new( "nettest" );

module:add_files( "source/*.*" );
module:add_files( "nettest.lua" );

module:add_dependency( "config" );
module:add_dependency( "base" );
module:add_dependency( "net" );

local project = Project:new(
	"nettest",
	"7199737d-373c-440d-a955-3f17889ec005",
	{ "x32", "x64" },
	{ "Debug", "Release" },
	module,
	ProjectTypes.consoleApplication
);