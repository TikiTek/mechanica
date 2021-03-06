#include "tiki/tool_project/project.hpp"

#include "tiki/tool_base/directory_tool.hpp"
#include "tiki/tool_project/package.hpp"

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
		m_isValid = loadFromFile();
	}

	Project::Project( const Path& projectPath )
	{
		m_projectPath = projectPath;
		m_contentPath = projectPath;
		m_contentPath.push( "content" );
		m_assetBuildPath = projectPath;
		m_assetBuildPath.push( "asset_build" );

		m_packagePool.create( 4u );
		m_isValid = loadFromFile();
	}

	Project::~Project()
	{
		while( !m_packages.isEmpty() )
		{
			Package* pPackage = m_packages.getFirst();
			m_packages.removeSortedByValue( pPackage );
			m_packagePool.removeUnsortedByValue( pPackage );
		}

		m_packagePool.dispose();
	}

	void Project::addPlatform( PlatformType value )
	{
		if( !m_platforms.contains( value ) )
		{
			m_platforms.pushBack( value );
		}
	}

	void Project::removePlatform( PlatformType value )
	{
		m_platforms.removeSortedByValue( value );
	}

	void Project::addShaderIncludeDirectory( const Path& value )
	{
		if( m_shaderIncludeDirectories.contains( value ) )
		{
			return;
		}

		m_shaderIncludeDirectories.add( value );
	}

	void Project::removeShaderIncludeDirectory( const Path& value )
	{
		m_shaderIncludeDirectories.removeSortedByValue( value );
	}

	Package* Project::addPackage( const DynamicString& packageName )
	{
		Path filename = m_contentPath;
		filename.setBasename( packageName.cStr() );
		filename.setExtension( ".package" );

		Package* pPackage = m_packagePool.push();
		pPackage->create( filename );
		return pPackage;
	}

	Package* Project::findPackage( const DynamicString& packageName )
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

	const Package* Project::findPackage( const DynamicString& packageName ) const
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

		XmlElement* pRootNode = document.createChild( "tikiproject" );

		pRootNode->createChild( "name" )->setValue( m_name.cStr() );
		pRootNode->createChild( "description" )->setValue( m_description.cStr() );
		pRootNode->createChild( "version" )->setValue( m_version.cStr() );

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

	bool Project::loadFromFile()
	{
		Path filePath = m_projectPath;
		filePath.push( "tikiproject.xml" );

		bool ok = true;
		XmlDocument document;
		if( document.loadFromFile( filePath.getCompletePath() ) )
		{
			const XmlElement* pRoot = document.findFirstChild( "tikiproject" );
			if( pRoot == nullptr )
			{
				TIKI_TRACE_ERROR( "[project] 'tikiproject' root node not found in '%s'.\n", filePath.getCompletePath() );
				ok = false;
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

				const XmlElement* pShaderIncludesNode = pRoot->findFirstChild( "shader_includes" );
				if( pShaderIncludesNode == nullptr )
				{
					TIKI_TRACE_ERROR( "[project] 'shader_includes' node not found in '%s'.\n", filePath.getCompletePath() );
				}
				else
				{
					for( const XmlElement* pShaderIncludeNode = pShaderIncludesNode->findFirstChild( "shader_include" ); pShaderIncludeNode != nullptr; pShaderIncludeNode = pShaderIncludeNode->findNextSiblingWithSameName() )
					{
						m_shaderIncludeDirectories.add().setCombinedPath( m_projectPath, pShaderIncludeNode->getValue() );
					}
				}
			}
		}
		else
		{
			ok = false;
		}

		List< Path > packageFiles;
		directory::findFiles( packageFiles, m_contentPath, ".package", false );

		for( const Path& packageFilePath : packageFiles )
		{
			Package* pPackage = m_packagePool.push();
			pPackage->create( packageFilePath );
			m_packages.push( pPackage );
		}

		for( Package& package : m_packages )
		{
			package.load( *this );
		}

		return ok;
	}
}