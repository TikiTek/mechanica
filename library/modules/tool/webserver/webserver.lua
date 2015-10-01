-- library/modules/tool/webserver

local module = Module:new( "webserver" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );

module:add_dependency( "base" );
module:add_dependency( "toolbase" );
module:add_dependency( "mongoose" );
module:add_dependency( "net" );
