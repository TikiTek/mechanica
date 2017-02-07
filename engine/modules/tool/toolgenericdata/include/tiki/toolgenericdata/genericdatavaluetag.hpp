#pragma once
#ifndef TIKI_GENERICDATAVALUETAG_HPP_INCLUDED
#define TIKI_GENERICDATAVALUETAG_HPP_INCLUDED

#include "tiki/base/basicstring.hpp"
#include "tiki/base/types.hpp"

namespace tiki
{
	class GenericDataValueTag
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataValueTag );

	public:

		explicit					GenericDataValueTag( GenericDataTypeCollection& typeCollection );
									~GenericDataValueTag();

		const string&				getTag() const;
		void						setTag( const string& tag );

		const string&				getContent() const;
		void						setContent( const string& content );

		GenericDataValueTag*		getChildTag() const;
		const GenericDataValueTag*	getChildTag();
		void						setChildTag( GenericDataValueTag* pChildTag );

		bool						parseTagString( const string& text );
		string						writeTagString() const;

	private:

		GenericDataTypeCollection&	m_typeCollection;

		string						m_tag;
		string						m_content;
		GenericDataValueTag*		m_pChildTag;
	};
}

#endif // TIKI_GENERICDATAVALUETAG_HPP_INCLUDED
