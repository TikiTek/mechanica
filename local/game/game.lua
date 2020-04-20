local module = Module:new();

module:add_files( "*.lua" );

module:add_files( "source/*.cpp" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );

module:add_dependency( "config" );
module:add_dependency( "core/base" );
module:add_dependency( "core/generic_data" );
module:add_dependency( "runtime/game_application" );
module:add_dependency( "runtime/game_flow" );
module:add_dependency( "runtime/physics_2d" );

module:add_dependency( "runtime/mechanica_game_states" );
