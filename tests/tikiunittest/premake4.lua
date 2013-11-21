-- library/tests/unittest

local module = Module:new( "tikiunittest" );

module:add_files( "source/*.*" );

module:add_dependency( "config" );
module:add_dependency( "base" );
module:add_dependency( "unittest" );

local project = Project:new(
	"tikiunittest",
	{ "x32", "x64" },
	{ "Debug", "Release" },
	module,
	ProjectTypes.consoleApplication
);