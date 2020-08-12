-- library/modules/runtime/script

local module = Module:new( "script" );

module:add_files( "include/tiki/**/*.*" );
module:add_files( "source/*.*" );
module:add_files( "script.lua" );
module:add_include_dir( "include" );

module:add_dependency( "base" );
module:add_dependency( "math" );
module:add_dependency( "lua" );
