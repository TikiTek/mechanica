-- library/modules/runtime/qtapplication

local module = Module:new( "qtapplication" );

module:add_files( "include/**/*.*" );
module:add_include_dir( "include" );
module:add_files( "*.lua" );
module:add_files( "source/*.*" );

module:add_dependency( "base" );
module:add_dependency( "qt" );
module:add_dependency( "framework" );
