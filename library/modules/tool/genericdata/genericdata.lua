-- library/modules/tool/genericdata

local module = Module:new( "genericdata" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );

module:add_dependency( "toolbase" );
