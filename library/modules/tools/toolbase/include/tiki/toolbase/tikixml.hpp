#pragma once
#ifndef TIKI_TIKIXML_HPP
#define TIKI_TIKIXML_HPP

#include "tiki/base/types.hpp"
#include "tiki/base/string.hpp"

#include "xml.h"

namespace tiki
{
	typedef List< const XmlElement* > XmlElementList;

	class TikiXml
	{
		friend void* xmlAlloc(size_t _bytes);

	public:

							TikiXml();

		void				create( const string& fileName );
		void				dispose();

		const XmlElement*	getRoot() const { return m_pNode; }

		const XmlElement*	findNodeByName( const string& name ) const;
		const XmlElement*	findFirstChild( const string& name,  const XmlElement* pElement ) const;
		const XmlElement*	findNext( const XmlElement* pElement ) const;
		const XmlElement*	findNext( const string& name, const XmlElement* pElement ) const;

		const XmlAttribute*	findAttributeByName( const string& name, const XmlElement* pElement ) const;

		size_t				getChilds( XmlElementList& targetList, const XmlElement* pElement, const string& name ) const;

	private:

		XmlElement*			m_pNode;
		void*				m_pData;

		static TikiXml*		s_pAllocInst;

	};
}


#endif // TIKI_TIKIXML_HPP
