local module = Module:new( "libraryunittests" );

module:add_files( "source/*.*" );
module:add_files( "libraryunittests.lua" );

module:add_dependency( "config" );
module:add_dependency( "base" );
module:add_dependency( "math" );
module:add_dependency( "webserver" );
module:add_dependency( "unittest" );

local project = Project:new(
	"libraryunittests",
	"a6c711be-42f0-4142-a368-85fe662b4fe0",
	{ "x32", "x64" },
	{ "Debug", "Release" },
	module,
	ProjectTypes.consoleApplication
);