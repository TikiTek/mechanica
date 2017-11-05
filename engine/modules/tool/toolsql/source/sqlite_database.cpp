#include "tiki/toolsql/sqlite_database.hpp"

#include "sqlite/sqlite3.h"

namespace tiki
{
	SqliteDatabase::SqliteDatabase()
	{
		m_pDatabase = nullptr;
		m_pLastError = nullptr;
	}

	SqliteDatabase::~SqliteDatabase()
	{
		TIKI_ASSERT( m_pDatabase == nullptr );
	}

	bool SqliteDatabase::create( const char* pFileName )
	{
		if ( sqlite3_open( pFileName, &m_pDatabase ) != SQLITE_OK )
		{
			m_pDatabase = nullptr;
			return false;
		}
		return true;
	}

	void SqliteDatabase::dispose()
	{
		if ( m_pDatabase != nullptr )
		{
			sqlite3_close( m_pDatabase );
			m_pDatabase = nullptr;
		}
	}

	bool SqliteDatabase::executeCommand( const char* pSql )
	{
		TIKI_ASSERT( m_pDatabase != nullptr );
		return sqlite3_exec( m_pDatabase, pSql, nullptr, nullptr, (char**)&m_pLastError ) == SQLITE_OK;
	}

	uint SqliteDatabase::getLastInsertId() const
	{
		TIKI_ASSERT( m_pDatabase != nullptr );
		return (uint)sqlite3_last_insert_rowid( m_pDatabase );
	}
}
