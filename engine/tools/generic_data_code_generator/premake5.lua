-- engine/tools/generic_data_code_generator

include( "../../.." );

local project = Project:new( "generic_data_code_generator", ProjectTypes.ConsoleApplication );

project:add_files( "source/*.*" );
project:add_files( "*.lua" );

project:add_dependency( "config" );
project:add_dependency( "tool/tool_base" );
project:add_dependency( "tool/tool_generic_data" );

project:add_install( "*.exe", "../../buildtools/generic_data_code_generator", "Release" );

finalize_default_solution( project )
