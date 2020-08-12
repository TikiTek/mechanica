-- library/modules/tool/unittest

local module = Module:new( "unittest" );

module:add_files( "include/**/*.*" );
module:add_files( "source/*.*" );
module:add_files( "unittest.lua" );
module:add_include_dir( "include" );

module:add_dependency( "base" );
module:add_dependency( "toolbase" );
