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

		const DynamicString&			getName() const { return m_name; }
		void							setName( const DynamicString& value ) { m_name = value; }

		const DynamicString&			getDescription() const { return m_description; }
		void							setDescription( const DynamicString& value ) { m_description = value; }

		const DynamicString&			getVersion() const { return m_version; }
		void							setVersion( const DynamicString& value ) { m_version = value; }

		const List< PlatformType >&		getPlatforms() const { return m_platforms; }
		void							addPlatform( PlatformType value );
		void							removePlatform( PlatformType value );

		const List< Path >&				getShaderIncludeDirectories() const { return m_shaderIncludeDirectories;}
		void							addShaderIncludeDirectory( const Path& value );
		void							removeShaderIncludeDirectory( const Path& value );

		Package*						addPackage( const DynamicString& packageName );
		Package*						findPackage( const DynamicString& packageName );
		const Package*					findPackage( const DynamicString& packageName ) const;
		const LinkedList< Package >&	getPackages() const;

		bool							writeToFile();

	private:

		bool							m_isValid;

		Path							m_projectPath;
		Path							m_contentPath;
		Path							m_assetBuildPath;

		DynamicString					m_name;
		DynamicString					m_description;
		DynamicString					m_version;
		List< PlatformType >			m_platforms;
		List< Path >					m_shaderIncludeDirectories;

		ChunkedPool< Package >			m_packagePool;
		LinkedList< Package >			m_packages;

		bool							loadFromFile();
	};
}
