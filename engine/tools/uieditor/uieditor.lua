-- library/tools/uieditor

local module = Module:new( "uieditor" );

module:add_files( "include/**/*.*" );
module:add_include_dir( "include" );
module:add_files( "uieditor.lua" );
module:add_files( "source/*.*" );

module:add_dependency( "config" );
module:add_dependency( "toolbase" );
module:add_dependency( "toolapplication" );
module:add_dependency( "toolui" );

module:set_define( "TIKI_BUILD_TOOLS", "TIKI_ON" );

local project = Project:new(
	"uieditor",
	"ce188937-8610-427a-9397-fc75e2d584c3",
	{ "x32", "x64" },
	{ "Debug", "Release" },
	module,
	ProjectTypes.windowApplication
);