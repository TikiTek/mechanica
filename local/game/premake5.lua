-- local/game

include( "../.." )

local project = Project:new( "game", ProjectTypes.WindowApplication )

project:add_files( "*.lua" )

project:add_files( "source/*.cpp" )
project:add_files( "include/**/*.hpp" )
project:add_include_dir( "include" )

project:add_dependency( "config" )
project:add_dependency( "core/base" )
project:add_dependency( "core/generic_data" )
project:add_dependency( "runtime/game_application" )
project:add_dependency( "runtime/game_flow" )
project:add_dependency( "runtime/physics_2d" )

project:add_dependency( "runtime/mechanica_game_states" )

finalize_default_solution( project )
