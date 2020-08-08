local module = Module:new( "toolui" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "toolui.lua" );
module:add_include_dir( "include" );

module:add_dependency( "base" );
module:add_dependency( "ui" );
module:add_dependency( "toolbase" );
module:add_dependency( "toolgenericdata" );
