-- library/modules/tool/xmltools

local module = Module:new( "xmltools" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "base" );
module:add_dependency( "tinyxml2" );
