-- library/modules/libraries/converter

local converter_project = Project:new(
	"converterlibrary",
	"a7beb386-b92c-4c98-9250-c9394bd86b42",
	{ "x32", "x64" },
	{ "Debug", "Release", "Master" },
	find_module( "tool/converterlibrary" ),
	ProjectTypes.sharedLibrary
);

local module = Module:new( "converter" );

module:add_files( "include/**/*.hpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module.import_func = function( project, solution )
	if not table.contains( solution.projects, converter_project ) and project ~= converter_project then 
		project:add_library_file( "converterlibrary" );
		solution:add_project( converter_project );
	end
end