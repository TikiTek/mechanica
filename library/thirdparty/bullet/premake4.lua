-- library/thirdparty/bullet

local module = Module:new( "bullet" );

module:add_include_dir( "src" );

module:add_files( "src/BulletSoftBody/*.*" );
module:add_files( "src/BulletDynamics/*.*" );
module:add_files( "src/BulletCollision/*.*" );
module:add_files( "src/LinearMath/*.*" );

module:add_files( "src/Bullet3Dynamics/*.*" );
module:add_files( "src/Bullet3Common/*.*" );
module:add_files( "src/Bullet3Geometry/*.*" );
module:add_files( "src/Bullet3Collision/*.*" );
module:add_files( "src/Bullet3Serialize/Bullet2FileLoader/*.*" );

module:add_files( "src/Bullet3OpenCL/*.*" );
