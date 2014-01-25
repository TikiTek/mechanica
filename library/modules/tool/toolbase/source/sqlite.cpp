
#include "tiki/toolbase/sqlite.hpp"

#include "sqlite/sqlite3.h"

namespace tiki
{
	SqliteDatabase::SqliteDatabase() 
	{
		m_pDatabase = nullptr;
	}

	SqliteDatabase::~SqliteDatabase()
	{
		TIKI_ASSERT( m_pDatabase == nullptr );
	}

	bool SqliteDatabase::create( const string& fileName )
	{
		if ( sqlite3_open( fileName.cStr(), &m_pDatabase ) != SQLITE_OK )
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

	bool SqliteDatabase::executeCommand( const string& sql )
	{
		char* pErrorMessage = nullptr;
		if ( sqlite3_exec( m_pDatabase, sql.cStr(), nullptr, nullptr, &pErrorMessage ) != SQLITE_OK )
		{
			m_lastError = pErrorMessage;
			return false;
		}
		return true;
	}


	SqliteQuery::SqliteQuery()
	{
		m_pQuery = nullptr;
	}

	SqliteQuery::~SqliteQuery()
	{
		TIKI_ASSERT( m_pQuery == nullptr );
	}

	bool SqliteQuery::create( const SqliteDatabase& dataBase, const string& sql )
	{
		TIKI_ASSERT( m_pQuery == nullptr );

		const char* pErrorMessage = nullptr;
		if ( sqlite3_prepare( dataBase.m_pDatabase, sql.cStr(), sql.getLength(), &m_pQuery, &pErrorMessage ) != SQLITE_OK )
		{
			m_lastError = pErrorMessage;
			return false;
		}

		const int fieldCount = sqlite3_column_count( m_pQuery );
		for (int i = 0u; i < fieldCount; ++i)
		{
			const string fieldName = sqlite3_column_name( m_pQuery, i );
			m_colunmNameMapping[ fieldName ] = i;
		} 

		return true;
	}

	void SqliteQuery::dispose()
	{
		if ( m_pQuery != nullptr )
		{
			sqlite3_free( m_pQuery );
			m_pQuery = nullptr;
		}
	}

	bool SqliteQuery::nextRow()
	{
		TIKI_ASSERT( m_pQuery != nullptr );
		return sqlite3_step( m_pQuery ) == SQLITE_ROW;
	}

	int SqliteQuery::getIntegerField( uint fieldIndex ) const
	{
		TIKI_ASSERT( m_pQuery != nullptr );
		return sqlite3_column_int( m_pQuery, (int)fieldIndex );
	}

	int SqliteQuery::getIntegerField( const string& fieldName ) const
	{
		TIKI_ASSERT( m_pQuery != nullptr );
		return sqlite3_column_int( m_pQuery, m_colunmNameMapping[ fieldName ] );
	}

	string SqliteQuery::getTextField( uint fieldIndex ) const
	{
		TIKI_ASSERT( m_pQuery != nullptr );
		return (const char*)sqlite3_column_text( m_pQuery, (int)fieldIndex );
	}

	string SqliteQuery::getTextField( const string& fieldName ) const
	{
		TIKI_ASSERT( m_pQuery != nullptr );
		return (const char*)sqlite3_column_text( m_pQuery, m_colunmNameMapping[ fieldName ] );
	}
}