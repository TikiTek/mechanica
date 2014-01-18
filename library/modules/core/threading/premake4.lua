-- library/modules/core/threading

local module = Module:new( "threading" );

module:add_files( "source/win/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );
