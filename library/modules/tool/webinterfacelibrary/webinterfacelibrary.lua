-- library/modules/tool/webinterfacelibrary

local module = Module:new( "webinterfacelibrary" );

module:add_files( "source/*.*" );
module:add_files( "webinterfacelibrary.lua" );

module:add_dependency( "config" );
module:add_dependency( "threading" );
module:add_dependency( "toollibraries" );
module:add_dependency( "webserver" );

local project = Project:new(
	"webinterfacelibrary",
	{ "x32", "x64" },
	{ "Debug", "Release", "Master" },
	module,
	ProjectTypes.sharedLibrary
);