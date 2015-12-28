-- library/modules/tool/converterbase

local module = Module:new( "converterbase" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "converterbase.lua" );
module:add_include_dir( "include" );

module:add_dependency( "base" );
module:add_dependency( "sqlite" );
module:add_dependency( "toolbase" );
module:add_dependency( "threading" );
module:add_dependency( "resource" );
module:add_dependency( "tasksystem" );
