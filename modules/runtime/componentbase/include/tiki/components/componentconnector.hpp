#pragma once
#ifndef __TIKI_COMPONENTCONNECTOR_HPP_INCLUDED__
#define __TIKI_COMPONENTCONNECTOR_HPP_INCLUDED__

#include "tiki/base/types.hpp"

namespace tiki
{
	template<typename T>
	struct ComponentReadConnector
	{
	public:

		const T*	getData() const { return m_pData; }

	private:

		const T*	m_pData;

	};
}

#endif // __TIKI_COMPONENTCONNECTOR_HPP_INCLUDED__
