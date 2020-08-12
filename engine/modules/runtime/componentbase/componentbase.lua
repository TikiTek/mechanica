-- library/modules/runtime/componentbase

local module = Module:new( "componentbase" );

module:add_files( "include/**/*.hpp" );
module:add_files( "source/*.*" );
module:add_files( "componentbase.lua" );
module:add_include_dir( "include" );

module:add_dependency( "base" );
