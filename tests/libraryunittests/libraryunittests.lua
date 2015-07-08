-- library/tests/libraryunittests

local module = Module:new( "libraryunittests" );

module:add_files( "source/*.*" );

module:add_dependency( "config" );
module:add_dependency( "base" );
module:add_dependency( "math" );
module:add_dependency( "webserver" );
module:add_dependency( "unittest" );

local project = Project:new(
	"libraryunittests",
	{ "x32", "x64" },
	{ "Debug", "Release" },
	module,
	ProjectTypes.consoleApplication
);