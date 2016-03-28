-- library/tools/editor

local module = Module:new( "editor" );

module:add_files( "source/*.*" );
module:add_files( "editornative.lua" );
module:add_files( "include/**/*.hpp" );

module:add_include_dir( "include" );

module:add_dependency( "config" );
module:add_dependency( "threading" );
module:add_dependency( "toolbase" );
module:add_dependency( "toollibraries" );
module:add_dependency( "resource" );
module:add_dependency( "graphics" );
module:add_dependency( "graphicsresources" );
module:add_dependency( "runtimeshared" );
module:add_dependency( "input" );
module:add_dependency( "editorbase" );
module:add_dependency( "genericdata" );
module:add_dependency( "toolapplication" );

module:add_library_file( "converterlibrary" );

local project = Project:new(
	"editor",
	{ "x32", "x64" },
	{ "Debug", "Release"},
	module,
	ProjectTypes.windowApplication
);
