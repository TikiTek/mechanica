-- library/modules/runtime/resource

local module = Module:new( "resource" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "resource.lua" );
module:add_include_dir( "include" );

module:add_dependency( "core/container" );
module:add_dependency( "core/io" );
module:add_dependency( "tool/toollibraries" );
