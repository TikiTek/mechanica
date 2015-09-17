#pragma once
#ifndef __TIKI_FORWARDDECLARATION_HPP_INCLUDED__
#define __TIKI_FORWARDDECLARATION_HPP_INCLUDED__

#define TIKI_FORWARD_CLASS( name ) class name
#define TIKI_FORWARD_ENUM( name ) enum name
#define TIKI_FORWARD_STRUCT( name ) struct name
#define TIKI_FORWARD_TEMPLATE( name ) template<typename T> class name

namespace tiki
{
	TIKI_FORWARD_TEMPLATE( Array );
	TIKI_FORWARD_TEMPLATE( SizedArray );
	TIKI_FORWARD_TEMPLATE( StaticArray );

	TIKI_FORWARD_TEMPLATE( BasicString );

	typedef BasicString< char > string;
}

#endif // __TIKI_FORWARDDECLARATION_HPP_INCLUDED__
