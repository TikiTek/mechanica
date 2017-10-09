
#include "tiki/converterbase/sqlite.hpp"

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
		return sqlite3_exec( m_pDatabase, pSql, nullptr, nullptr, (char**)&m_pLastError ) != SQLITE_OK;
	}

	uint SqliteDatabase::getLastInsertId() const
	{
		TIKI_ASSERT( m_pDatabase != nullptr );
		return (uint)sqlite3_last_insert_rowid( m_pDatabase );
	}

	SqliteQuery::SqliteQuery()
	{
		m_pQuery = nullptr;
		m_pLastError = nullptr;
	}

	SqliteQuery::~SqliteQuery()
	{
		dispose();
	}

	bool SqliteQuery::create( const SqliteDatabase& dataBase, const char* pSql )
	{
		TIKI_ASSERT( m_pQuery == nullptr );

		if ( sqlite3_prepare( dataBase.m_pDatabase, pSql, (int)getStringLength( pSql ), &m_pQuery, &m_pLastError ) != SQLITE_OK )
		{
			return false;
		}

		const int fieldCount = sqlite3_column_count( m_pQuery );
		m_colunmNames.create( fieldCount );

		for (int i = 0u; i < fieldCount; ++i)
		{
			m_colunmNames[ i ] = sqlite3_column_name( m_pQuery, i );
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

	int SqliteQuery::getIntegerField( const char* pFieldName ) const
	{
		TIKI_ASSERT( m_pQuery != nullptr );
		return sqlite3_column_int( m_pQuery, findColunmIndexByName( pFieldName ) );
	}

	const char* SqliteQuery::getTextField( uint fieldIndex ) const
	{
		TIKI_ASSERT( m_pQuery != nullptr );
		return (const char*)sqlite3_column_text( m_pQuery, (int)fieldIndex );
	}

	const char* SqliteQuery::getTextField( const char* pFieldName ) const
	{
		TIKI_ASSERT( m_pQuery != nullptr );
		return (const char*)sqlite3_column_text( m_pQuery, findColunmIndexByName( pFieldName ) );
	}

	int SqliteQuery::findColunmIndexByName( const char* pFieldName ) const
	{
		for (uint i = 0u; i < m_colunmNames.getCount(); ++i)
		{
			if( isStringEquals( m_colunmNames[ i ], pFieldName ) )
			{
				return int( i );
			}
		}

		TIKI_ASSERT( false );
		return -1;
	}
}
