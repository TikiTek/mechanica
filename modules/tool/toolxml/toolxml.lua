-- library/modules/tool/toolxml

local module = Module:new( "toolxml" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "runtime/base" );
module:add_dependency( "thirdparty/tinyxml2" );
