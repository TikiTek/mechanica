-- library/modules/core/container

local module = Module:new( "container" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "container.lua" );
module:add_include_dir( "include" );

module:add_dependency( "core/base" );
module:add_dependency( "core/threading" );
module:add_dependency( "thirdparty/dbalsterxml" );
