-- library/tools/autoconverter

local module = Module:new( "autoconverter" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );

module:add_dependency( "config" );
module:add_dependency( "fontconverter" );
module:add_dependency( "shaderconverter" );
module:add_dependency( "textureconverter" );
module:add_dependency( "modelconverter" );

local project = Project:new(
	"autoconverter",
	{ "x32", "x64" },
	{ "Debug", "Release" },
	module,
	ProjectTypes.consoleApplication
);