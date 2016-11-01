-- library/modules/tool/converterbase

local module = Module:new( "converterbase" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "converterbase.lua" );
module:add_include_dir( "include" );

module:add_dependency( "core/base" );
module:add_dependency( "core/threading" );
module:add_dependency( "core/resource" );
module:add_dependency( "runtime/tasksystem" );
module:add_dependency( "tool/toolbase" );
module:add_dependency( "thirdparty/sqlite" );
