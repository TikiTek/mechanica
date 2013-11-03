-- library/modules/tools/textureexport

local module = Module:new( "textureexport" );

module:add_files( "source/*.*" );
module:add_files( "include/**/*.hpp" );
module:add_include_dir( "include" );

module:add_dependency( "libpsd" );
module:add_dependency( "opencv" );

module:add_library_file( "opencv_core246d", "Debug" );
module:add_library_file( "opencv_imgproc246d", "Debug" );
module:add_library_file( "opencv_highgui246d", "Debug" );

module:add_library_file( "opencv_core246", "Release" );
module:add_library_file( "opencv_imgproc246", "Release" );
module:add_library_file( "opencv_highgui246", "Release" );

module:add_binary_file( "opencv_core246d.dll", "Debug" );
module:add_binary_file( "opencv_imgproc246d.dll", "Debug" );
module:add_binary_file( "opencv_highgui246d.dll", "Debug" );

module:add_binary_file( "opencv_core246.dll", "Release" );
module:add_binary_file( "opencv_imgproc246.dll", "Release" );
module:add_binary_file( "opencv_highgui246.dll", "Release" );
