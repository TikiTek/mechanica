#pragma once
#ifndef __TIKI_SQLITE_HPP_INCLUDED__
#define __TIKI_SQLITE_HPP_INCLUDED__

#include "tiki/base/dynamic_string.hpp"
#include "tiki/base/types.hpp"

struct sqlite3;
struct sqlite3_stmt;

namespace tiki
{
	class SqliteQuery;

	class SqliteDatabase
	{
		TIKI_NONCOPYABLE_CLASS( SqliteDatabase );

	public:

		SqliteDatabase();
		~SqliteDatabase();
		friend class SqliteQuery;

		bool			create( const char* pFileName );
		void			dispose();

		bool			isCreated() const { return m_pDatabase != nullptr; }

		bool			executeCommand( const char* pSql );

		uint			getLastInsertId() const;
		const char*		getLastError() const { return m_pLastError; }

	private:

		sqlite3*		m_pDatabase;
		const char*		m_pLastError;

	};

	class SqliteQuery
	{
		TIKI_NONCOPYABLE_CLASS( SqliteQuery );

	public:

		SqliteQuery();
		~SqliteQuery();

		bool			create( const SqliteDatabase& dataBase, const char* pSql );
		void			dispose();

		bool			isCreated() const { return m_pQuery != nullptr; }

		bool			nextRow();

		int				getIntegerField( uint fieldIndex ) const;
		int				getIntegerField( const char* pFieldName ) const;
		const char*		getTextField( uint fieldIndex ) const;
		const char*		getTextField( const char* pFieldName ) const;

		const char*		getLastError() const { return m_pLastError; }

	private:

		sqlite3_stmt*			m_pQuery;
		const char*				m_pLastError;

		Array< const char* >	m_colunmNames;

		int						findColunmIndexByName( const char* pFieldName ) const;

	};
}

#endif // __TIKI_SQLITE_HPP_INCLUDED__
