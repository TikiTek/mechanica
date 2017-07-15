-- library/modules/runtime/particle

local module = Module:new( "particle" );

module:add_files( "include/**/*.*" );
module:add_files( "source/*.*" );
module:add_files( "particle.lua" );
module:add_include_dir( "include" );

module:add_dependency( "base" );
module:add_dependency( "math" );
