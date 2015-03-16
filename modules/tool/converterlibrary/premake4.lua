-- library/modules/tool/converterlibrary

local module = Module:new( "converterlibrary" );

module:add_files( "source/*.*" );

module:add_dependency( "config" );
module:add_dependency( "threading" );
module:add_dependency( "toollibraries" );
module:add_dependency( "fontconverter" );
module:add_dependency( "shaderconverter" );
module:add_dependency( "textureconverter" );
module:add_dependency( "modelconverter" );
module:add_dependency( "materialconverter" );
module:add_dependency( "animationconverter" );

local project = Project:new(
	"converterlibrary",
	{ "x32", "x64" },
	{ "Debug", "Release", "Master" },
	module,
	ProjectTypes.sharedLibrary
);