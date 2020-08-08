local asset_converter_library_project = Project:new(
	"asset_converter_library",
	{ "x32", "x64" },
	{ "Debug", "Release", "Master" },
	ProjectTypes.SharedLibrary
);

asset_converter_library_project:add_files( "source/*.*" );
asset_converter_library_project:add_files( "*.lua" );

asset_converter_library_project:add_dependency( "libraries/asset_converter_interface" );

asset_converter_library_project:add_dependency( "core/config" );
asset_converter_library_project:add_dependency( "core/threading" );
asset_converter_library_project:add_dependency( "tool/tool_project" );

asset_converter_library_project:add_dependency( "converters/font_converter" );
asset_converter_library_project:add_dependency( "converters/shader_converter" );
asset_converter_library_project:add_dependency( "converters/texture_converter" );
asset_converter_library_project:add_dependency( "converters/model_converter" );
asset_converter_library_project:add_dependency( "converters/animation_converter" );
asset_converter_library_project:add_dependency( "converters/generic_data_converter" );

local module = Module:new();

module.import_func = function( project, solution )
	project:add_library_file( "asset_converter_library" );
	solution:add_project( asset_converter_library_project );
end