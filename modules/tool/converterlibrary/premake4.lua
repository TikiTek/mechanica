-- library/modules/tool/converterlibrary

local module = Module:new( "converterlibrary" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );

module:add_dependency( "config" );
module:add_dependency( "resource" );
module:add_dependency( "fontconverter" );
module:add_dependency( "shaderconverter" );
module:add_dependency( "textureconverter" );
module:add_dependency( "modelconverter" );
module:add_dependency( "materialconverter" );

local project = Project:new(
	"converterlibrary",
	{ "x32", "x64" },
	{ "Debug", "Release" },
	module,
	ProjectTypes.sharedLibrary
);