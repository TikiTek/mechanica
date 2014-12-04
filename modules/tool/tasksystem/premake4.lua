-- library/modules/tool/tasksystem

local module = Module:new( "tasksystem" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );

module:add_dependency( "base" );
module:add_dependency( "toolbase" );
module:add_dependency( "threading" );
