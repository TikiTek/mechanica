#pragma once

#include "tiki/base/dynamic_string.hpp"

struct sqlite3;
struct sqlite3_stmt;

namespace tiki
{
	class SqliteQuery;

	class SqliteDatabase
	{
		TIKI_NONCOPYABLE_CLASS( SqliteDatabase );
		friend class SqliteQuery;

	public:

						SqliteDatabase();
						~SqliteDatabase();

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
}
