#ifndef TIKI_INPUTSYSTEM_HPP__
#define TIKI_INPUTSYSTEM_HPP__

#include "tiki/base/fixedsizedarray.hpp"
#include "tiki/base/platform.hpp"
#include "tiki/base/types.hpp"
#include "tiki/input/inputdevice.hpp"
#include "tiki/input/inputevent.hpp"

#if TIKI_ENABLED( TIKI_SDL )
#	include "../../../source/sdl/inputsystem_sdl.hpp"
#elif TIKI_ENABLED( TIKI_BUILD_MSVC )
#	include "../../../source/dinput/inputsystem_dinput.hpp"
#else
#	error Platform not supported
#endif

namespace tiki
{
	struct InputSystemParameters
	{
		InputSystemParameters()
		{
			windowHandle	= InvalidWindowHandle;
			instanceHandle	= InvalidInstanceHandle;
		}

		WindowHandle	windowHandle;
		InstanceHandle	instanceHandle;
	};
	
	class InputSystem
	{
		friend class InputSystemPlatform;
		TIKI_NONCOPYABLE_CLASS( InputSystem );
		
	public:

		enum
		{
			MaxInputDeviceCount	= 8u,
			MaxInputEventCount	= 64u
		};

		typedef FixedSizedArray< InputDevice, MaxInputDeviceCount > InputDeviceArray;
		typedef FixedSizedArray< InputEvent, MaxInputEventCount > InputEventArray;

								InputSystem();
								~InputSystem();

		bool					create( const InputSystemParameters& params );
		void					dispose();

		void					update();
		void					endFrame();

		uint					getDeviceCount() const;
		InputDevice&			getDeviceByIndex( uint index ) const;

		uint					getEventCount() const;
		bool					popEvent( InputEvent& inputEvent );

	private:
			
		InputDeviceArray		m_devices;
		InputEventArray			m_events;

		InputSystemPlatformData	m_platformData;

		void					connectDevice( const InputDevice& device );
		void					disconnectDevice( const InputDevice& device );

	};
}
#endif // TIKI_INPUTSYSTEM_HPP__
