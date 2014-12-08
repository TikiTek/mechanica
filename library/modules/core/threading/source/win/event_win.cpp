
#include "tiki/threading/event.hpp"

#include "tiki/base/assert.hpp"

namespace tiki
{
	Event::Event()
	{
		m_platformData.eventHandle = INVALID_HANDLE_VALUE;
	}

	Event::~Event()
	{
		TIKI_ASSERT( m_platformData.eventHandle == INVALID_HANDLE_VALUE );
	}

	void Event::create( bool initialState /*= false*/, bool manualReset /*= false */, const char* pName /*= nullptr*/ )
	{
		TIKI_ASSERT( m_platformData.eventHandle == INVALID_HANDLE_VALUE );

		m_platformData.eventHandle = CreateEventA(
			nullptr,
			manualReset,
			initialState,
			pName
		);
	}

	void Event::dispose()
	{
		if ( m_platformData.eventHandle != INVALID_HANDLE_VALUE )
		{
			CloseHandle( m_platformData.eventHandle );
			m_platformData.eventHandle = INVALID_HANDLE_VALUE;
		}
	}

	void Event::signal()
	{
		TIKI_ASSERT( m_platformData.eventHandle != INVALID_HANDLE_VALUE );

		TIKI_VERIFY( SetEvent( m_platformData.eventHandle ) );
	}

	void Event::reset()
	{
		TIKI_ASSERT( m_platformData.eventHandle != INVALID_HANDLE_VALUE );

		TIKI_VERIFY( ResetEvent( m_platformData.eventHandle ) );
	}

	bool Event::waitForSignal( uint timeOut /*= TimeOutInfinity */ )
	{
		TIKI_ASSERT( m_platformData.eventHandle != INVALID_HANDLE_VALUE );

		if ( WaitForSingleObject( m_platformData.eventHandle, DWORD( timeOut ) ) == WAIT_TIMEOUT )
		{
			return false;
		}

		return true;
	}
}