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
		void					addDependencies( Package* pPackage );
		void					removeDependencies( Package* pPackage );

		const Path&				getGenericTypesPath() const { return m_genericTypesPath; }
		void					setGenericTypesPath( const Path& value ) { m_genericTypesPath = value; }
		const Path&				getGenericDataPath() const { return m_genericDataPath; }
		void					setGenericDataPath( const Path& value ) { m_genericDataPath = value; }
		const Path&				getAssetTemplatesPath() const { return m_assetTemplatesPath; }
		void					setAssetTemplatesPath( const Path& value ) { m_assetTemplatesPath = value; }

		void					findGenericDataTypeFiles( List< Path >& targetList ) const;
		void					findAssetTemplateFiles( List< Path >& targetList ) const;

	private: // friend

		bool					create( const Path& filePath );
		void					create( const Path& contentPath, const DynamicString& packageName );

		bool					writeToFile() const;

	private:

		DynamicString			m_name;
		Path					m_filename;
		Path					m_basePath;

		DynamicString			m_description;
		DynamicString			m_author;

		List< Package* >		m_dependencies;

		Path					m_genericTypesPath;
		Path					m_genericDataPath;
		Path					m_assetTemplatesPath;
	};
}
