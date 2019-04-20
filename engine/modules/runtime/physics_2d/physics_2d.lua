local module = Module:new();

module:add_files( "*.lua" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );

module:add_dependency( "core/math" );
module:add_dependency( "third_party/box2d" );
