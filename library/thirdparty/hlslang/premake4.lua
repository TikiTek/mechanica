-- library/thirdparty/hlslang

local module = Module:new( "hlslang" );

module.module_type = ModuleTypes.FilesModule;

module:add_files( "source/GLSLCodeGen/*.*" );
module:add_files( "source/Include/*.*" );
module:add_files( "source/MachineIndependent/*.*" );
module:add_files( "source/MachineIndependent/preprocessor/*.*" );
module:add_include_dir( "include" );
module:add_include_dir( "source/MachineIndependent" );

if is_windows then
	module:add_files( "source/OSDependent/Windows/*.*" );
	module:add_include_dir( "source/OSDependent/Windows" );
end
