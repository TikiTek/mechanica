-- library/modules/tool/modelexport

local module = Module:new( "modelexport" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "modelexport.lua" );
module:add_include_dir( "include" );

module:add_dependency( "core/math" );
