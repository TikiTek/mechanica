-- library/modules/tool/converterlibrary

local module = Module:new( "converterlibrary" );

module:add_files( "source/*.*" );
module:add_files( "converterlibrary.lua" );

module:add_dependency( "core/config" );
module:add_dependency( "core/threading" );
module:add_dependency( "libraries/converter" );
module:add_dependency( "converters/fontconverter" );
module:add_dependency( "converters/shaderconverter" );
module:add_dependency( "converters/textureconverter" );
module:add_dependency( "converters/modelconverter" );
module:add_dependency( "converters/animationconverter" );
module:add_dependency( "converters/genericdataconverter" );

local project = Project:new(
	"converterlibrary",
	"a7beb386-b92c-4c98-9250-c9394bd86b42",
	{ "x32", "x64" },
	{ "Debug", "Release", "Master" },
	module,
	ProjectTypes.sharedLibrary
);