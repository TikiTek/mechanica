#pragma once

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

		const DynamicString&	getTag() const;
		void					setTag( const DynamicString& tag );

		const DynamicString&	getContent() const;
		void					setContent( const DynamicString& content );

		GenericDataTag*			getChildTag() const;
		const GenericDataTag*	getChildTag();
		void					setChildTag( GenericDataTag* pChildTag );

		bool					parseTagString( const DynamicString& rawText );
		DynamicString			toString() const;

		static bool				isTagString( const DynamicString& rawText );

	private:

		DynamicString			m_tag;
		DynamicString			m_content;
		GenericDataTag*			m_pChildTag;
	};
}
