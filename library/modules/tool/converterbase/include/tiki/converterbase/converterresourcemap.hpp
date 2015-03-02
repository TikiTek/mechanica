#pragma once
#ifndef __TIKI_CONVERTERRESOURCEMAP_HPP_INCLUDED__
#define __TIKI_CONVERTERRESOURCEMAP_HPP_INCLUDED__

#include "tiki/base/string.hpp"
#include "tiki/base/types.hpp"
#include "tiki/toolbase/map.hpp"

namespace tiki
{
	class ConverterResourceMap
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( ConverterResourceMap );

	public:

		void			create( const string& mapFilename );
		void			dispose();

		void			registerResource( const string& fileName );

		void			writeToFile();

	private:

		typedef Map< crc32, string > ResourceMap;

		string			m_fileName;
		ResourceMap		m_map;

	};
}

#endif // __TIKI_CONVERTERRESOURCEMAP_HPP_INCLUDED__
