#pragma once
#ifndef TIKI_GENERICDATAARRAY_HPP_INCLUDED__
#define TIKI_GENERICDATAARRAY_HPP_INCLUDED__

#include "tiki/base/basicstring.hpp"
#include "tiki/base/types.hpp"
#include "tiki/toolbase/list.hpp"

struct _XmlElement;

namespace tiki
{
	class GenericDataTypeArray;
	class GenericDataTypeCollection;
	class GenericDataValue;
	class ResourceWriter;
	class XmlReader;

	class GenericDataArray
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataArray );

	public:

										GenericDataArray( GenericDataTypeCollection& collection );
										~GenericDataArray();

		bool							create( const GenericDataTypeArray* pType );
		void							dispose();

		const GenericDataTypeArray*		getType() const;

		uint							getCount() const;

		GenericDataValue&				getElement( uint index );
		const GenericDataValue&			getElement( uint index ) const;

		bool							addElement( const GenericDataValue& value );
		bool							setElement( uint index, const GenericDataValue& value );
		bool							removeElement( uint index );

		bool							importFromXml( XmlReader& reader, const _XmlElement* pArrayRootNode );

		// TODO: bool					exportToXml( XmlWriter& writer ) const;
		bool							exportToResource( ResourceWriter& writer ) const;

	private:

		GenericDataTypeCollection&		m_collection;

		const GenericDataTypeArray*		m_pType;

		List< GenericDataValue >		m_array;

	};
}

#endif // TIKI_GENERICDATAARRAY_HPP_INCLUDED__
