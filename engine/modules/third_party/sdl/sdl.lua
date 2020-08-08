local module = Module:new( "sdl" );

module.module_type = ModuleTypes.UnityCModule;

module:add_files( "sdl.lua" );
module:add_files( "SDL_config.h" );
module:add_include_dir( "." );
module:set_base_path( "blobs/sdl" );

module:add_files( "src/*.c" );
module:add_files( "src/*.h" );
module:add_files( "src/atomic/*.c" );
module:add_files( "src/audio/*.c" );
module:add_files( "src/audio/*.h" );
module:add_files( "src/audio/disk/*.c" );
module:add_files( "src/audio/disk/*.h" );
module:add_files( "src/audio/dummy/*.c" );
module:add_files( "src/audio/dummy/*.h" );
module:add_files( "src/cpuinfo/*.c" );
module:add_files( "src/events/*.c" );
module:add_files( "src/events/*.h" );
module:add_files( "src/file/*.c" );
module:add_files( "src/haptic/*.c" );
module:add_files( "src/haptic/*.h" );
module:add_files( "src/joystick/*.c" );
module:add_files( "src/joystick/*.h" );
module:add_files( "src/libm/*.c" );
module:add_files( "src/libm/*.h" );
module:add_files( "src/power/*.c" );
module:add_files( "src/render/*.c" );
module:add_files( "src/render/*.h" );
module:add_files( "src/render/opengl/*.c" );
module:add_files( "src/render/opengl/*.h" );
module:add_files( "src/render/opengles/*.c" );
module:add_files( "src/render/opengles/*.h" );
module:add_files( "src/render/software/*.c" );
module:add_files( "src/render/software/*.h" );
module:add_files( "src/stdlib/*.c" );
module:add_files( "src/test/*.c" );
module:add_files( "src/thread/*.c" );
module:add_files( "src/thread/*.h" );
module:add_files( "src/thread/generic/SDL_syscond.c" );
module:add_files( "src/timer/*.c" );
module:add_files( "src/timer/*.h" );
module:add_files( "src/video/*.c" );
module:add_files( "src/video/*.h" );
module:add_files( "src/video/dummy/*.c" );
module:add_files( "src/video/dummy/*.h" );

if is_windows then
	module:add_files( "src/audio/winmm/*.c" );
	module:add_files( "src/audio/winmm/*.h" );
	module:add_files( "src/core/windows/*.c" );
	module:add_files( "src/core/windows/*.h" );
	module:add_files( "src/loadso/windows/*.c" );
	module:add_files( "src/main/windows/*.c" );
	module:add_files( "src/power/windows/*.c" );
	module:add_files( "src/thread/windows/*.c" );
	module:add_files( "src/thread/windows/*.h" );
	module:add_files( "src/timer/windows/*.c" );
	module:add_files( "src/video/windows/*.c" );
	module:add_files( "src/video/windows/*.h" );
	
	if use_msvc then
		module:add_files( "src/audio/directsound/*.c" );
		module:add_files( "src/audio/directsound/*.h" );
		module:add_files( "src/audio/xaudio2/*.c" );
		module:add_files( "src/haptic/windows/*.c" );
		module:add_files( "src/joystick/windows/*.c" );
		module:add_files( "src/joystick/windows/*.h" );
		module:add_files( "src/render/direct3d/*.c" );
		module:add_files( "src/render/opengles2/*.c" );
		module:add_files( "src/render/opengles2/*.h" );
	end
	
	module:add_dependency( "directx" );
	
	module:add_library_file( "Imm32" );
	module:add_library_file( "Winmm" );
	module:add_library_file( "Version" );
elseif is_linux then

else
	throw "Platform not implemented"
end

module:add_files( "include/*.h" );
module:add_include_dir( "include" );
