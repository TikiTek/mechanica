#include "tiki/toolproject/project.hpp"

#include "tiki/toolbase/directory_tool.hpp"
#include "tiki/toolproject/package.hpp"

namespace tiki
{
	Project::Project()
	{
		Path projectPath = platform::getExecutablePath();
		while( !projectPath.isEmpty() )
		{
			Path filePath = projectPath;
			filePath.push( "tikiproject.xml" );

			if( file::exists( filePath.getCompletePath() ) )
			{
				break;
			}

			projectPath.pop();
		}

		m_projectPath = projectPath;
		m_contentPath = projectPath;
		m_contentPath.push( "content" );
		m_assetBuildPath = projectPath;
		m_assetBuildPath.push( "asset_build" );

		m_packagePool.create( 4u );
		loadFromFile();
	}

	Project::Project( const Path& projectPath )
	{
		m_projectPath = projectPath;
		m_contentPath = projectPath;
		m_contentPath.push( "content" );
		m_assetBuildPath = projectPath;
		m_assetBuildPath.push( "asset_build" );

		m_packagePool.create( 4u );
		loadFromFile();
	}

	Project::~Project()
	{
		while( !m_packages.isEmpty() )
		{
			Package& package = m_packages.getFirst();
			m_packages.removeSortedByValue( package );
			m_packagePool.removeUnsortedByValue( package );
		}

		m_packagePool.dispose();
	}

	Package* Project::addPackage( const string& packageName )
	{
		Package* pPackage = m_packagePool.push();
		pPackage->create( m_contentPath, packageName );
		return pPackage;
	}

	Package* Project::findPackage( const string& packageName )
	{
		for( Package& package : m_packages )
		{
			if( package.getName() == packageName )
			{
				return &package;
			}
		}

		return nullptr;
	}

	const Package* Project::findPackage( const string& packageName ) const
	{
		return const_cast< Project* >( this )->findPackage( packageName );
	}

	const LinkedList< Package >& Project::getPackages() const
	{
		return m_packages;
	}

	bool Project::writeToFile()
	{
		Path filePath = m_projectPath;
		filePath.push( "tikiproject.xml" );

		XmlDocument document;

		XmlElement* pNameNode = document.createElement( "name" );
		pNameNode->setValue( m_name.cStr() );

		XmlElement* pDescriptionNode = document.createElement( "description" );
		pDescriptionNode->setValue( m_description.cStr() );

		XmlElement* pVersionNode = document.createElement( "version" );
		pVersionNode->setValue( m_version.cStr() );

		XmlElement* pRootNode = document.createElement( "tikiproject" );
		pRootNode->appendChild( pNameNode );
		pRootNode->appendChild( pDescriptionNode );
		pRootNode->appendChild( pVersionNode );

		document.appendChild( pRootNode );
		if( !document.saveToFile( filePath.getCompletePath() ) )
		{
			TIKI_TRACE_ERROR( "[project] Failed to write '%s'.\n", filePath.getCompletePath() );
			return false;
		}

		for( const Package& package : m_packages )
		{
			if( !package.writeToFile() )
			{
				return false;
			}
		}

		return true;
	}

	void Project::loadFromFile()
	{
		Path filePath = m_projectPath;
		filePath.push( "tikiproject.xml" );

		XmlDocument document;
		if( document.loadFromFile( filePath.getCompletePath() ) )
		{
			const XmlElement* pRoot = document.findFirstChild( "tikiproject" );
			if( pRoot == nullptr )
			{
				TIKI_TRACE_ERROR( "[project] 'tikiproject' root node not found in '%s'.\n", filePath.getCompletePath() );
			}
			else
			{
				const XmlElement* pNameNode = pRoot->findFirstChild( "name" );
				if( pNameNode == nullptr )
				{
					TIKI_TRACE_ERROR( "[project] 'name' node not found in '%s'.\n", filePath.getCompletePath() );
				}
				else
				{
					m_name = pNameNode->getValue();
				}

				const XmlElement* pDescriptionNode = pRoot->findFirstChild( "description" );
				if( pDescriptionNode == nullptr )
				{
					TIKI_TRACE_ERROR( "[project] 'description' node not found in '%s'.\n", filePath.getCompletePath() );
				}
				else
				{
					m_description = pDescriptionNode->getValue();
				}

				const XmlElement* pVersionNode = pRoot->findFirstChild( "version" );
				if( pVersionNode == nullptr )
				{
					TIKI_TRACE_ERROR( "[project] 'version' node not found in '%s'.\n", filePath.getCompletePath() );
				}
				else
				{
					m_version = pVersionNode->getValue();
				}

				const XmlElement* pPlatformsNode = pRoot->findFirstChild( "platforms" );
				if( pPlatformsNode == nullptr )
				{
					TIKI_TRACE_ERROR( "[project] 'platforms' node not found in '%s'.\n", filePath.getCompletePath() );
				}
				else
				{
					 // TODO
				}
			}
		}

		List< Path > packageFiles;
		directory::findFiles( packageFiles, m_contentPath, ".package", false );

		for( const Path& packageFilePath : packageFiles )
		{
			Package* pPackage = m_packagePool.push();
			if( !pPackage->create( packageFilePath ) )
			{
				m_packagePool.removeUnsortedByValue( *pPackage );
				continue;
			}

			m_packages.push( pPackage );
		}
	}
}