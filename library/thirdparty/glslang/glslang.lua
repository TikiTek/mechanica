-- library/thirdparty/glslang

local module = Module:new( "glslang" );

module.module_type = ModuleTypes.FilesModule;

module:add_files( "GenericCodeGen/*.*" );
module:add_files( "Include/*.*" );
module:add_files( "MachineIndependent/*.*" );
module:add_files( "MachineIndependent/preprocessor/*.*" );
module:add_files( "Public/*.*" );
module:add_files( "OGLCompilersDLL/*.*" );
module:add_files( "glslang.lua" );
module:add_include_dir( "Public" );
module:add_include_dir( "MachineIndependent" );
module:add_include_dir( "OGLCompilersDLL" );
module:add_include_dir( "." );

if is_windows then
	module:add_files( "OSDependent/Windows/*.*" );
	module:add_include_dir( "OSDependent/Windows" );
end
