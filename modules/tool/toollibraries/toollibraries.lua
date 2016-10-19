-- library/modules/tool/toollibraries

local module = Module:new( "toollibraries" );

module:add_files( "include/**/*.hpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "config" );
module:add_dependency( "base" );
