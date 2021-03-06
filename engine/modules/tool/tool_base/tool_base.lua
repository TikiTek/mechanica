local module = Module:new();

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "core/base" );
module:add_dependency( "core/container" );

module:add_external( "https://github.com/TikiTek/thirdparty_trex.git" )

module:set_define( "TIKI_BUILD_TOOLS", "TIKI_ON" );
