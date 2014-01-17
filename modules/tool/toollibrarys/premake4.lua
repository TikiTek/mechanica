-- library/modules/tool/toollibrarys

local module = Module:new( "toollibrarys" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );

module:add_dependency( "config" );
module:add_dependency( "resource" );
