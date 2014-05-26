-- local/modules/gamecomponentdata

local module = Module:new( "gamecomponentdata" );

module:add_files( "include/**/*.hpp" );
module:add_files( "source/*.cpp" );
module:add_include_dir( "include" );

module:add_dependency( "base" );
module:add_dependency( "componentbase" );
