local asset_converter_library_project = Project:new( "asset_converter_library", ProjectTypes.SharedLibrary );

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

local module = Module:new( "asset_converter_library" );

module.import_func = function( project, solution )
	if project ~= asset_converter_library_project then
		project:add_project_dependency( asset_converter_library_project )
	end
	
	solution:add_project( asset_converter_library_project );
end