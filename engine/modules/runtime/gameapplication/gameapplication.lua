-- library/modules/runtime/gameapplication

local module = Module:new( "gameapplication" );

module:add_files( "include/**/*.*" );
module:add_include_dir( "include" );
module:add_files( "*.lua" );
module:add_files( "source/*.*" );

module:add_dependency( "core/base" );
module:add_dependency( "runtime/framework" );

module:set_define( "TIKI_BUILD_TOOLS", "TIKI_OFF" );
