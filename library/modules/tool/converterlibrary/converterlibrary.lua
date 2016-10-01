-- library/modules/tool/converterlibrary

local module = Module:new( "converterlibrary" );

module:add_files( "source/*.*" );
module:add_files( "converterlibrary.lua" );

module:add_dependency( "config" );
module:add_dependency( "threading" );
module:add_dependency( "toollibraries" );
module:add_dependency( "fontconverter" );
module:add_dependency( "shaderconverter" );
module:add_dependency( "textureconverter" );
module:add_dependency( "modelconverter" );
module:add_dependency( "animationconverter" );
module:add_dependency( "genericdataconverter" );

local project = Project:new(
	"converterlibrary",
	"a7beb386-b92c-4c98-9250-c9394bd86b42",
	{ "x32", "x64" },
	{ "Debug", "Release", "Master" },
	module,
	ProjectTypes.sharedLibrary
);