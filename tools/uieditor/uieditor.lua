-- library/tools/uieditor

local module = Module:new( "uieditor" );

module:add_files( "source/*.*" );

module:add_dependency( "config" );
module:add_dependency( "toolbase" );
module:add_dependency( "toolgenericdata" );
module:add_dependency( "converterbase" );

module:set_define( "TIKI_BUILD_TOOLS", "TIKI_ON" );

local project = Project:new(
	"uieditor",
	{ "x32", "x64" },
	{ "Debug", "Release" },
	module,
	ProjectTypes.windowApplication
);