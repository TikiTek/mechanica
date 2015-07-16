#pragma once
#ifndef TIKI_GENERICDATADOCUMENT_HPP_INCLUDED__
#define TIKI_GENERICDATADOCUMENT_HPP_INCLUDED__

#include "tiki/base/basicstring.hpp"
#include "tiki/base/types.hpp"
#include "tiki/toolbase/map.hpp"

namespace tiki
{
	class GenericDataObject;
	class ResourceWriter;
	class XmlReader;

	class GenericDataDocument
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataDocument );

	public:

		GenericDataDocument();
		~GenericDataDocument();

		void							addObject( const string& pName, GenericDataObject* pObject );

		GenericDataObject*				getObject();
		const GenericDataObject*		getObject() const;

		bool							importFromXml( XmlReader& reader );

		// TODO: bool							exportToXml() const;
		bool							exportToResource( ResourceWriter& writer ) const;

	private:

		Map<string, GenericDataObject*>	m_objects;

	};
}

#endif // TIKI_GENERICDATADOCUMENT_HPP_INCLUDED__
