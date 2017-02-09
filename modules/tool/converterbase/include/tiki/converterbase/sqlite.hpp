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

		bool			executeCommand( const string& sql );

		uint			getLastInsertId() const;
		const string&	getLastError() const { return m_lastError; }

	private:

		sqlite3*		m_pDatabase;
		string			m_lastError;

	};

	class SqliteQuery
	{
		TIKI_NONCOPYABLE_CLASS( SqliteQuery );

	public:

		SqliteQuery();
		~SqliteQuery();

		bool			create( const SqliteDatabase& dataBase, const string& sql );
		void			dispose();

		bool			isCreated() const { return m_pQuery != nullptr; }

		bool			nextRow();

		int				getIntegerField( uint fieldIndex ) const;
		int				getIntegerField( const string& fieldName ) const;
		string			getTextField( uint fieldIndex ) const;
		string			getTextField( const string& fieldName ) const;

		const string&	getLastError() const { return m_lastError; }

	private:

		sqlite3_stmt*			m_pQuery;
		string					m_lastError;

		Array< string >			m_colunmNames;

		int						findColunmIndexByName( const string& fieldName ) const;

	};
}

#endif // __TIKI_SQLITE_HPP_INCLUDED__
