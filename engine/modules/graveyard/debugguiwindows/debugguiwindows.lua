-- library/modules/runtime/debugguiwindows

local module = Module:new( "debugguiwindows" );

module:add_files( "include/**/*.hpp" );
module:add_files( "source/*.*" );
module:add_files( "debugguiwindows.lua" );
module:add_include_dir( "include" );

module:add_dependency( "debuggui" );
