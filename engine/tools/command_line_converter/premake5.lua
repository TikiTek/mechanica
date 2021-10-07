-- engine/tools/command_line_converter

include( "../../.." );

local project = Project:new( "command_line_converter", ProjectTypes.ConsoleApplication );

project:add_files( "source/*.*" );
project:add_files( "*.lua" );

project:add_dependency( "config" );
project:add_dependency( "core/threading" );
project:add_dependency( "tool/asset_converter_interface" );

finalize_default_solution( project )
