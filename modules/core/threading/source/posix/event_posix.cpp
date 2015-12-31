#include "tiki/threading/event.hpp"

#include "tiki/base/assert.hpp"

namespace tiki
{
	Event::Event()
	{
	}

	Event::~Event()
	{
	}

	bool Event::create( bool initialState /*= false*/, bool manualReset /*= false */, const char* pName /*= nullptr*/ )
	{
		error;
	}

	void Event::dispose()
	{
	}

	void Event::signal()
	{
	}

	void Event::reset()
	{
	}

	bool Event::waitForSignal( uint timeOut /*= TimeOutInfinity */ )
	{
	}
}