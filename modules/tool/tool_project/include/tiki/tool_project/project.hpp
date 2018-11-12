#pragma once

#include "tiki/base/dynamic_string.hpp"
#include "tiki/base/path.hpp"
#include "tiki/base/platform.hpp"
#include "tiki/container/chunked_pool.hpp"
#include "tiki/container/linked_list.hpp"
#include "tiki/container/list.hpp"

namespace tiki
{
	class Package;

	class Project
	{
		TIKI_NONCOPYABLE_CLASS( Project );

	public:

										Project();
										Project( const Path& projectPath );
										~Project();

		bool							isValid() const { return m_isValid; }

		const Path&						getProjectPath() const { return m_projectPath; }
		const Path&						getContentPath() const { return m_contentPath; }
		const Path&						getAssetBuildPath() const { return m_assetBuildPath; }

		const string&					getName() const { return m_name; }
		void							setName( const string& value ) { m_name = value; }

		const string&					getDescription() const { return m_description; }
		void							setDescription( const string& value ) { m_description = value; }

		const string&					getVersion() const { return m_version; }
		void							setVersion( const string& value ) { m_version = value; }

		const List< PlatformType >&		getPlatforms() const { return m_platforms; }
		void							addPlatform( PlatformType value );
		void							removePlatform( PlatformType value );

		Package*						addPackage( const string& packageName );
		Package*						findPackage( const string& packageName );
		const Package*					findPackage( const string& packageName ) const;
		const LinkedList< Package >&	getPackages() const;

		bool							writeToFile();

	private:

		bool					m_isValid;

		Path					m_projectPath;
		Path					m_contentPath;
		Path					m_assetBuildPath;

		string					m_name;
		string					m_description;
		string					m_version;
		List< PlatformType >	m_platforms;

		ChunkedPool< Package >	m_packagePool;
		LinkedList< Package >	m_packages;

		bool					loadFromFile();
	};
}
