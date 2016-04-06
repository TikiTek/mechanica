-- library/thirdparty/bullet

local module = Module:new( "bullet" );

module.module_type = ModuleTypes.FilesModule;

--module:add_files( "src/BulletCollision/*" );
--module:add_files( "src/BulletDynamics/*" );
module:add_files( "src/BulletSoftBody/*" );
module:add_files( "src/LinearMath/*" );
module:add_files( "src/BulletCollision/BroadphaseCollision/*" );
module:add_files( "src/BulletCollision/CollisionDispatch/*" );
module:add_files( "src/BulletCollision/CollisionShapes/*" );
module:add_files( "src/BulletCollision/Gimpact/*" );
module:add_files( "src/BulletCollision/NarrowPhaseCollision/*" );
module:add_files( "src/BulletDynamics/Character/*" );
module:add_files( "src/BulletDynamics/ConstraintSolver/*" );
module:add_files( "src/BulletDynamics/Dynamics/*" );
module:add_files( "src/BulletDynamics/Featherstone/*" );
module:add_files( "src/BulletDynamics/MLCPSolvers/*" );
module:add_files( "src/BulletDynamics/Vehicle/*" );

module:add_files( "bullet.lua" );

module:add_include_dir( "src" );
