-- library/modules/tool/toolbase

local module = Module:new( "toolbase" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );

module:add_dependency( "dbalsterxml" );

	module:set_define( "TIKI_BUILD_TOOLS", "TIKI_ON" );
