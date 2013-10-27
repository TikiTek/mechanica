-- library/modules/tools/textureexport

local module = Module:new( "textureexport" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );

module:add_dependency( "libpsd" );
module:add_dependency( "opencv" );

module:add_library( "opencv_core246d" );
module:add_library( "opencv_imgproc246d" );
module:add_library( "opencv_highgui246d" );
