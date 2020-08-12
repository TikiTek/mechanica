-- library/modules/tool/toolgenericdata

local module = Module:new( "toolgenericdata" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "toolgenericdata.lua" );
module:add_include_dir( "include" );

module:add_dependency( "toolbase" );
