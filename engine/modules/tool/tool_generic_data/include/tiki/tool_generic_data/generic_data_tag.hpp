#pragma once
#ifndef TIKI_GENERICDATAVALUETAG_HPP_INCLUDED
#define TIKI_GENERICDATAVALUETAG_HPP_INCLUDED

#include "tiki/base/dynamic_string.hpp"
#include "tiki/base/types.hpp"

namespace tiki
{
	class GenericDataTag
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataTag );

	public:

		explicit				GenericDataTag();
		explicit				GenericDataTag( const GenericDataTag* pCopyFrom );
								~GenericDataTag();

		const string&			getTag() const;
		void					setTag( const string& tag );

		const string&			getContent() const;
		void					setContent( const string& content );

		GenericDataTag*			getChildTag() const;
		const GenericDataTag*	getChildTag();
		void					setChildTag( GenericDataTag* pChildTag );

		bool					parseTagString( const string& rawText );
		string					writeTagString() const;

		static bool				isTagString( const string& rawText );

	private:

		string					m_tag;
		string					m_content;
		GenericDataTag*			m_pChildTag;
	};
}

#endif // TIKI_GENERICDATAVALUETAG_HPP_INCLUDED
