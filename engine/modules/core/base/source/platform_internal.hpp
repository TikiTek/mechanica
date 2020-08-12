#pragma once
#ifndef TIKI_PLATFORM_INTERNAL_HPP_INCLUDED__
#define TIKI_PLATFORM_INTERNAL_HPP_INCLUDED__

#if TIKI_DISABLED( TIKI_BUILD_LIBRARY )
namespace tiki
{
	namespace platform
	{
		int startApplication();
	}
}
#endif

#endif // TIKI_PLATFORM_INTERNAL_HPP_INCLUDED__
