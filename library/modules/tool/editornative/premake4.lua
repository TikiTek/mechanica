-- library/modules/tool/editornative

local module = Module:new( "editornative" );

module:add_files( "source/*.*" );
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

module:add_library_file( "converterlibrary" );

module:set_flag( "Managed", nil, nil );

local project = Project:new(
	"editornative",
	{ "x32", "x64" },
	{ "Debug", "Release"},
	module,
	ProjectTypes.sharedLibrary
);

-- ignore warning
project.buildoptions = "/wd4793";