#include "tiki/tool_generic_data/generic_data_document_collection.hpp"

#include "tiki/io/file.hpp"
#include "tiki/tool_project/package.hpp"
#include "tiki/tool_project/project.hpp"

namespace tiki
{
	GenericDataDocumentCollection::GenericDataDocumentCollection( GenericDataTypeCollection& typeCollection )
		: m_typeCollection( typeCollection )
	{
	}

	GenericDataDocumentCollection::~GenericDataDocumentCollection()
	{
		dispose();
	}

	void GenericDataDocumentCollection::create( const Project& project )
	{
		m_pProject = &project;
	}

	void GenericDataDocumentCollection::dispose()
	{
		for( uint i = 0u; i < m_documents.getCount(); ++i )
		{
			TIKI_DELETE( m_documents.getValueAt( i ).pDocument );
		}
		m_documents.dispose();
	}

	GenericDataDocument* GenericDataDocumentCollection::loadDocument( const char* pFilename )
	{
		const DynamicString filename = pFilename;
		const DocumentMap::InsertResult insertResult = m_documents.insertKey( filename );
		if( insertResult.isNew )
		{
			GenericDataDocument* pDocument = TIKI_NEW( GenericDataDocument )( *this );

			Path filePath;
			for( const Package& package : m_pProject->getPackages() )
			{
				if( filename.startsWith( package.getGenericDataPath().getCompletePath(), AsciiCompareType::CaseInsensitive ) )
				{
					filePath.setCompletePath( pFilename );
				}
				else
				{
					filePath.setCombinedPath( package.getGenericDataPath(), pFilename );
				}

				if( !file::exists( filePath.getCompletePath() ) )
				{
					continue;
				}

				if( !pDocument->importFromFile( filePath.getCompletePath() ) )
				{
					TIKI_TRACE_ERROR( "[GenericDataDocumentCollection::loadDocument] Unable to load '%s'.\n", filePath.getCompletePath() );
					TIKI_DELETE( pDocument );
					m_documents.remove( filename );
					return nullptr;
				}

				insertResult.pValue->pPackage	= &package;
				insertResult.pValue->pDocument	= pDocument;
				break;
			}

			if( insertResult.pValue->pPackage == nullptr ||
				insertResult.pValue->pDocument == nullptr )
			{
				TIKI_TRACE_ERROR( "[GenericDataDocumentCollection::loadDocument] Unable to find '%s'.\n", pFilename );
				TIKI_DELETE( pDocument );
				m_documents.remove( filename );
				return nullptr;
			}
		}

		return insertResult.pValue->pDocument;
	}
}