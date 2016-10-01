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
	"bb5d3b50-3b21-446c-9fed-001c74b273f2",
	{ "x32", "x64" },
	{ "Debug", "Release", "Master" },
	module,
	ProjectTypes.sharedLibrary
);