-- library/modules/tool/toolproject

local module = Module:new( "toolproject" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "core/base" );
