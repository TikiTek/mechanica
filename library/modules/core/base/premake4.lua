-- library/modules/core/base

local module = Module:new( "base" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );

module:set_define( "TIKI_DEFAULT_ALIGNMENT", "16" );
