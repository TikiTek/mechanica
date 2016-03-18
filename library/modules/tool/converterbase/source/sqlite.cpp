
#include "tiki/converterbase/sqlite.hpp"

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

	bool SqliteDatabase::executeCommand( const string& sql )
	{
		TIKI_ASSERT( m_pDatabase != nullptr );

		char* pErrorMessage = nullptr;
		if ( sqlite3_exec( m_pDatabase, sql.cStr(), nullptr, nullptr, &pErrorMessage ) != SQLITE_OK )
		{
			m_lastError = pErrorMessage;
			return false;
		}

		return true;
	}

	uint SqliteDatabase::getLastInsertId() const
	{
		TIKI_ASSERT( m_pDatabase != nullptr );
		return (uint)sqlite3_last_insert_rowid( m_pDatabase );
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
		if ( sqlite3_prepare( dataBase.m_pDatabase, sql.cStr(), int( sql.getLength() ), &m_pQuery, &pErrorMessage ) != SQLITE_OK )
		{
			m_lastError = pErrorMessage;
			return false;
		}

		const int fieldCount = sqlite3_column_count( m_pQuery );
		m_colunmNames.create( fieldCount );

		for (int i = 0u; i < fieldCount; ++i)
		{
			const string fieldName = sqlite3_column_name( m_pQuery, i );
			m_colunmNames[ i ] = fieldName;
		}

		return true;
	}

	void SqliteQuery::dispose()
	{
		if ( m_pQuery != nullptr )
		{
			sqlite3_finalize( m_pQuery );
			m_pQuery = nullptr;
		}

		m_colunmNames.dispose();
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
		return sqlite3_column_int( m_pQuery, findColunmIndexByName( fieldName ) );
	}

	string SqliteQuery::getTextField( uint fieldIndex ) const
	{
		TIKI_ASSERT( m_pQuery != nullptr );
		return (const char*)sqlite3_column_text( m_pQuery, (int)fieldIndex );
	}

	string SqliteQuery::getTextField( const string& fieldName ) const
	{
		TIKI_ASSERT( m_pQuery != nullptr );
		return (const char*)sqlite3_column_text( m_pQuery, findColunmIndexByName( fieldName ) );
	}

	int SqliteQuery::findColunmIndexByName( const string& fieldName ) const
	{
		for (uint i = 0u; i < m_colunmNames.getCount(); ++i)
		{
			if ( m_colunmNames[ i ] == fieldName )
			{
				return int( i );
			}
		}

		TIKI_ASSERT( false );
		return -1;
	}

}
