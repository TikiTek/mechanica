local asset_converter_library_project = Project:new(
	"asset_converter_library",
	"a7beb386-b92c-4c98-9250-c9394bd86b42",
	{ "x32", "x64" },
	{ "Debug", "Release", "Master" },
	find_module( "tool/asset_converter_library" ),
	ProjectTypes.sharedLibrary
);

local module = Module:new( "asset_converter_interface" );

module:add_files( "include/**/*.hpp" );
module:add_files( "source/*.cpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module.import_func = function( project, solution )
	if not table.contains( solution.projects, asset_converter_library_project ) and project ~= asset_converter_library_project then 
		project:add_library_file( "asset_converter_library" );
		solution:add_project( asset_converter_library_project );
	end
end