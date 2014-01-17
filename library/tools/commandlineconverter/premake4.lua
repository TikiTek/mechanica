-- library/tools/commandlineconverter

local module = Module:new( "commandlineconverter" );

module:add_files( "source/*.*" );

module:add_dependency( "config" );
module:add_dependency( "toollibrarys" );

module:set_define( "TIKI_BUILD_TOOLS", "TIKI_OFF" );

module:add_library_file( "converterlibrary" );

local project = Project:new(
	"commandlineconverter",
	{ "x32", "x64" },
	{ "Debug", "Release" },
	module,
	ProjectTypes.consoleApplication
);