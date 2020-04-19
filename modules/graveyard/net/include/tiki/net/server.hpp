#pragma once
#ifndef TIKI_TIKIXML_HPP
#define TIKI_TIKIXML_HPP

#include "tiki/base/string.hpp"
#include "tiki/base/types.hpp"
#include "tiki/container/array.hpp"

#include "xml.h"

namespace tiki
{
	typedef Array< const XmlElement* > XmlElementList;

	class XmlReader
	{
		TIKI_NONCOPYABLE_CLASS( XmlReader );
		friend void* xmlAlloc(uintreg _bytes, void* pUserData);

	public:

							XmlReader();
							~XmlReader();

		bool				create( const char* pFileName );
		void				dispose();

		const XmlElement*	getRoot() const { return m_pNode; }

		const XmlElement*	findNodeByName( const char* pName ) const;
		const XmlElement*	findFirstChild( const char* pName,  const XmlElement* pElement ) const;
		const XmlElement*	findNext( const XmlElement* pElement ) const;
		const XmlElement*	findNext( const char* pName, const XmlElement* pElement ) const;

		const XmlAttribute*	findAttributeByName( const char* pName, const XmlElement* pElement ) const;

		uintreg				getChilds( XmlElementList& targetList, const XmlElement* pElement, const char* pName ) const;

	private:

		XmlElement*			m_pNode;
		void*				m_pData;

	};
}


#endif // TIKI_TIKIXML_HPP
