-- local/game

local module = Module:new( "game" );

module:add_files( "source/*.cpp" );
module:add_files( "include/**/*.hpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "config" );
module:add_dependency( "core/base" );
module:add_dependency( "core/genericdata" );
module:add_dependency( "runtime/gameapplication" );
module:add_dependency( "runtime/gameflowsystem" );
module:add_dependency( "runtime/physics2d" );

module:add_dependency( "gamestates" );


--module:add_library_file( "webinterfacelibrary" );

