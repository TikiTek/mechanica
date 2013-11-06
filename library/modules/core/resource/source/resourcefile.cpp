
#include "tiki/resource/resourcefile.hpp"

#include "tiki/base/functions.hpp"

namespace tiki
{
	Endianness resource::getEndianness( const ResourceFileHeader& header )
	{
		return Endianness_Little;
	}

	const void* resource::getSectionData( const SectionHeader& header, const void* pBase )
	{
		return addPtr( pBase, ( sizeof( ReferenceItem ) * header.referenceCount ) );
	}

	ReferenceType resource::getReferenceType( const ReferenceItem& item )
	{
		return (ReferenceType)( item.type_offsetInSection & 0xc0000000u );
	}

	uint resource::getReferenceOffsetInSection( const ReferenceItem& item )
	{
		return ( item.type_offsetInSection & 0x3fffffffu );
	}

	StringType resource::getStringType( const StringItem& item )
	{
		return ( StringType )( ( item.type_lengthModifier_textLength & 0xc0000000u ) >> 30u );
	}

	uint resource::getStringLengthModifier( const StringItem& item )
	{
		return ( ( item.type_lengthModifier_textLength & 0x40000000u ) >> 28u );
	}

	uint resource::getStringTextLength( const StringItem& item )
	{
		return ( item.type_lengthModifier_textLength & 0x0fffffffu );
	}

	uint resource::getStringLengthInBytes( const StringItem& item )
	{
		return getStringTextLength( item ) * getStringLengthModifier( item );
	}
}