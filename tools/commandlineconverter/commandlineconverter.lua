-- library/tools/commandlineconverter

local module = Module:new( "commandlineconverter" );

module:add_files( "source/*.*" );
module:add_files( "commandlineconverter.lua" );

module:add_dependency( "config" );
module:add_dependency( "toollibraries" );
module:add_dependency( "renderer2dshader" );

module:set_define( "TIKI_BUILD_TOOLS", "TIKI_OFF" );

module:add_library_file( "converterlibrary" );

local project = Project:new(
	"commandlineconverter",
	"575bd0eb-4477-443f-8b4f-5c8a1f72bf86",
	{ "x32", "x64" },
	{ "Debug", "Release" },
	module,
	ProjectTypes.consoleApplication
);