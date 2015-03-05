#pragma once
#ifndef TIKI_TIKIXML_HPP
#define TIKI_TIKIXML_HPP

#include "tiki/base/types.hpp"
#include "tiki/base/string.hpp"
#include "tiki/toolbase/list.hpp"

#include "xml.h"

namespace tiki
{
	typedef List< const XmlElement* > XmlElementList;

	class TikiXml
	{
		friend void* xmlAlloc(size_t _bytes, void* pUserData);

	public:

							TikiXml();
							~TikiXml();

		void				create( cstring pFileName );
		void				dispose();

		const XmlElement*	getRoot() const { return m_pNode; }

		const XmlElement*	findNodeByName( cstring pName ) const;
		const XmlElement*	findFirstChild( cstring pName,  const XmlElement* pElement ) const;
		const XmlElement*	findNext( const XmlElement* pElement ) const;
		const XmlElement*	findNext( cstring pName, const XmlElement* pElement ) const;

		const XmlAttribute*	findAttributeByName( cstring pName, const XmlElement* pElement ) const;

		size_t				getChilds( XmlElementList& targetList, const XmlElement* pElement, cstring pName ) const;

	private:

		XmlElement*			m_pNode;
		void*				m_pData;

	};
}


#endif // TIKI_TIKIXML_HPP
