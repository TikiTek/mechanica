#pragma once
#ifndef __TIKI_RESOURCENAMEMAPPER_HPP_INCLUDED__
#define __TIKI_RESOURCENAMEMAPPER_HPP_INCLUDED__

#include "tiki/base/sortedsizedmap.hpp"
#include "tiki/base/types.hpp"

namespace tiki
{
	class ResourceNameMapper
	{
		TIKI_NONCOPYABLE_CLASS( ResourceNameMapper );

	public:

		typedef SortedSizedMap< crc32, const char* > NameMap;

		ResourceNameMapper();
		~ResourceNameMapper();

		bool			create( const void* pBinaryData );
		void			dispose();

		const char*		getResourceName( crc32 crcName ) const;

#if TIKI_ENABLED( TIKI_BUILD_TOOLS )
		const NameMap&	getMap() const { return m_map; }
#endif

	private:

		NameMap			m_map;
		char*			m_pStringData;

	};
}

#endif // __TIKI_RESOURCENAMEMAPPER_HPP_INCLUDED__
