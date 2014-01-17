-- library/tools/commandlineconverter

local module = Module:new( "commandlineconverter" );

module:add_files( "source/*.*" );

module:add_dependency( "config" );
module:add_dependency( "resource" );

local project = Project:new(
	"commandlineconverter",
	{ "x32", "x64" },
	{ "Debug", "Release" },
	module,
	ProjectTypes.consoleApplication
);