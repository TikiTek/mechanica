#pragma once

#include "tiki/base/dynamic_string.hpp"
#include "tiki/base/path.hpp"
#include "tiki/container/linked_list.hpp"
#include "tiki/container/list.hpp"

namespace tiki
{
	class Project;

	class Package : public LinkedItem< Package >
	{
		friend class Project;

	public:

		const DynamicString&	getName() const { return m_name; }
		const Path&				getFilename() const { return m_filename; }
		const Path&				getBasepath() const { return m_basePath; }

		const DynamicString&	getDescription() const { return m_description; }
		void					setDescription( const DynamicString& value ) { m_description = value; }
		const DynamicString&	getAuthor() const { return m_author; }
		void					setAuthor( const DynamicString& value ) { m_author = value; }

		const List< Package* >&	getDependencies() const { return m_dependencies; }
		void					addDependency( Package* pPackage );
		void					removeDependency( Package* pPackage );

		const Path&				getGenericTypesPath() const { return m_genericTypesPath; }
		const Path&				getGenericDataPath() const { return m_genericDataPath; }
		const Path&				getAssetTemplatesPath() const { return m_assetTemplatesPath; }
		const DynamicString&	getGenericTypesString() const { return m_genericTypesString; }
		const DynamicString&	getGenericDataString() const { return m_genericDataString; }
		const DynamicString&	getAssetTemplatesString() const { return m_assetTemplatesString; }
		void					setGenericTypesString( const DynamicString& value );
		void					setGenericDataString( const DynamicString& value );
		void					setAssetTemplatesString( const DynamicString& value );

		void					findGenericDataTypeFiles( List< Path >& targetList ) const;
		void					findAssetTemplateFiles( List< Path >& targetList ) const;

		bool					writeToFile() const;

	private: // friend

		void					create( const Path& filePath );

		bool					load( Project& project );

	private:

		DynamicString			m_name;
		Path					m_filename;
		Path					m_basePath;

		DynamicString			m_description;
		DynamicString			m_author;

		List< Package* >		m_dependencies;

		DynamicString			m_genericTypesString;
		Path					m_genericTypesPath;
		DynamicString			m_genericDataString;
		Path					m_genericDataPath;
		DynamicString			m_assetTemplatesString;
		Path					m_assetTemplatesPath;
	};
}
