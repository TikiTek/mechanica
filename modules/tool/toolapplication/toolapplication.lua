-- library/modules/runtime/toolapplication

local module = Module:new( "toolapplication" );

module:add_files( "include/**/*.*" );
module:add_include_dir( "include" );
module:add_files( "toolapplication.lua" );
module:add_files( "source/*.*" );

module:add_dependency( "base" );
module:add_dependency( "framework" );
