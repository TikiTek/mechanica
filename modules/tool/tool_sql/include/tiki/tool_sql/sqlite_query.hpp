#pragma once
#ifndef TIKI_SQLITE_QUERY_HPP_INCLUDED
#define TIKI_SQLITE_QUERY_HPP_INCLUDED

#include "tiki/base/dynamic_string.hpp"

struct sqlite3_stmt;

namespace tiki
{
	class SqliteDatabase;

	class SqliteQuery
	{
		TIKI_NONCOPYABLE_CLASS( SqliteQuery );

	public:

								SqliteQuery();
								~SqliteQuery();

		bool					create( SqliteDatabase& dataBase, const char* pSql );
		void					dispose();

		bool					isCreated() const { return m_pQuery != nullptr; }

		bool					nextRow();

		int						getIntegerField( uint fieldIndex ) const;
		int						getIntegerField( const char* pFieldName ) const;
		sint64					getBigIntField( uint fieldIndex ) const;
		sint64					getBigIntField( const char* pFieldName ) const;
		const char*				getTextField( uint fieldIndex ) const;
		const char*				getTextField( const char* pFieldName ) const;

		const char*				getLastError() const { return m_pLastError; }

	private:

		sqlite3_stmt*			m_pQuery;
		const char*				m_pLastError;

		Array< const char* >	m_colunmNames;

		int						findColunmIndexByName( const char* pFieldName ) const;
	};
}

#endif // TIKI_SQLITE_QUERY_HPP_INCLUDED
