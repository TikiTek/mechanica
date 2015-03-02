#pragma once
#ifndef TIKI_KEYVALUEPAIR_HPP_INCLUDED__
#define TIKI_KEYVALUEPAIR_HPP_INCLUDED__

namespace tiki
{
	template<typename TKey, typename TValue>
	struct KeyValuePair
	{
		TKey	key;
		TValue	value;
	};
}

#endif // TIKI_KEYVALUEPAIR_HPP_INCLUDED__
