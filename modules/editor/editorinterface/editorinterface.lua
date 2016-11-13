-- library/modules/editor/editorinterface

local module = Module:new( "editorinterface" );

--module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_files( "*.lua" );
module:add_include_dir( "include" );

module:add_dependency( "core/base" );
