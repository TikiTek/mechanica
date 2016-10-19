-- library/modules/runtime/gameapplication

local module = Module:new( "gameapplication" );

module:add_files( "include/**/*.*" );
module:add_include_dir( "include" );
module:add_files( "*.lua" );
module:add_files( "source/*.*" );

module:add_dependency( "base" );
module:add_dependency( "framework" );
