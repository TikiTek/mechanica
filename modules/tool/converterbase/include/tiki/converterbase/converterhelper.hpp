#pragma once
#ifndef __TIKI_CONVERTERHELPER_HPP_INCLUDED__
#define __TIKI_CONVERTERHELPER_HPP_INCLUDED__

#include "tiki/base/assert.hpp"
#include "tiki/base/crc32.hpp"
#include "tiki/base/string.hpp"
#include "tiki/base/types.hpp"
#include "tiki/converterbase/resource_section_writer.hpp"

namespace tiki
{
	TIKI_FORCE_INLINE bool readResourceReference( ResourceSectionWriter& sectionWriter, const string& text, ReferenceKey& targetKey )
	{
		if ( text.isEmpty() == false && text.countSubstring( ":" ) >= 1u )
		{
			Array< string > components;
			text.split( components, ":" );
			TIKI_ASSERT( components.getCount() >= 2u );

			const string fileName		= components[ 1u ];
			const fourcc resourceType	= makeFourcc( components[ 0u ].cStr() );
			crc32 resourceKey			= 0u;

			if ( components.getCount() == 2u )
			{
				resourceKey = crcString( components[ 1u ] );
			}
			else
			{
				resourceKey = strtol( components[ 2u ].cStr(), nullptr, 16 );
			}

			components.dispose();

			targetKey = sectionWriter.addResourceLink( fileName, resourceKey, resourceType );
			return true;
		}
		else
		{
			return false;
		}
	}

	TIKI_FORCE_INLINE void writeResourceReference( ResourceSectionWriter& sectionWriter, const string& text )
	{
		ReferenceKey key;
		if ( readResourceReference( sectionWriter, text, key ) )
		{
			sectionWriter.writeReference( &key );
		}
		else
		{
			sectionWriter.writeReference( nullptr );
		}
	}
}

#endif // __TIKI_CONVERTERHELPER_HPP_INCLUDED__
